#include <world/chunk.hpp> // Chunk and ChunkManager include each other.
#include <world/chunkManager.hpp>



ChunkManager::ChunkManager(unsigned int seed){
    ChunkGeneration::init(seed);
    generateChunks({0,0});
}

void ChunkManager::generateChunks(const LocInt& loc, int distance){
    ChunkID chunkId = getChunkID(loc);
    for(int dx=-distance; dx<=distance; dx++){
        for(int dz=-distance; dz<=distance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            if(chunks.count(chunkCandidate)==0){
                // std::cout << "Generating chunk:\n";
                // auto start = std::chrono::high_resolution_clock::now();
                addChunk(chunkCandidate);
                chunks[chunkCandidate]->generateChunkTerrain();
                // auto end = std::chrono::high_resolution_clock::now();
                // auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                // std::cout << "Took: " << ms << " microseconds" << std::endl;
            }
        }
    }
}

void ChunkManager::generateChunks(const LocInt& loc){
    generateChunks(loc,chunkGenerationDistance);
}


void ChunkManager::generateChunksAsync(const LocInt& loc, int distance){
    ChunkID chunkId = getChunkID(loc);
    for(int dx=-distance; dx<=distance; dx++){
        for(int dz=-distance; dz<=distance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            if(chunks.count(chunkCandidate)==0){
                addChunk(chunkCandidate);
                // To do: add the chunk generation in async here!
                futureDump.push_back(std::async(std::launch::async, &Chunk::generateChunkTerrain, chunks.at(chunkCandidate).get()));
            }
        }
    }
}

void ChunkManager::generateChunksAsync(const LocInt& loc){
    generateChunksAsync(loc,chunkGenerationDistance);
}


ChunkID ChunkManager::getChunkID(const LocInt& loc) const{
    // Neat trick to round down to the nearest multiple of 16.
    // Only works if MAXCHUNKX=16!
    return {loc.x & ~15,loc.z & ~15};

}

LocInt ChunkManager::getLocWithinChunk(const LocInt& loc) const{
    // Only works if MAXCHUNKX=16!
    return {loc.x & 15,loc.y,loc.z & 15};
}

void ChunkManager::placeBlock(const LocInt& loc,const BlockID& blockId){
    for(int i = 0; i<6; i++){
        LocInt nb = loc + dirs[i];
        ChunkID nbChunkId = getChunkID(nb);
        if(chunks.count(nbChunkId)!=0){
            chunks.at(nbChunkId)->setDirty();
        }
    }
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        chunks.at(chunkID)->setBlockId(getLocWithinChunk(loc),blockId);
    }
}

void ChunkManager::deleteBlock(const LocInt& loc){
    for(int i = 0; i<6; i++){
        LocInt nb = loc + dirs[i];
        ChunkID nbChunkId = getChunkID(nb);
        if(chunks.count(nbChunkId)!=0){
            chunks.at(nbChunkId)->setDirty();
        }
    }
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        chunks.at(chunkID)->deleteBlock(getLocWithinChunk(loc));
    }
}
const Chunk& ChunkManager::getChunkPointer(const ChunkID& chunkID){
    if(chunks.count(chunkID)==0){
        std::cout << "ChunkId not in chunks: " << chunkID.x << "," << chunkID.z << std::endl;
    }
    assert(chunks.count(chunkID));
    return *chunks.at(chunkID);
}

BlockID ChunkManager::checkBlock(const LocInt& loc) const {

    // std::cout << "Inside checkBlock!" << std::endl;

    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        return chunks.at(chunkID)->getBlockId(getLocWithinChunk(loc));
    }
    return BlockID::Air;
}

bool ChunkManager::isSolid(const LocInt& loc) const {

    // ChunkID chunkID = {loc.x- (loc.x%MAXCHUNKX),loc.z- (loc.z%MAXCHUNKZ)};
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        return chunks.at(chunkID)->blockIsSolid(getLocWithinChunk(loc));
    }
    return false;

}

bool ChunkManager::isOpaque(const LocInt& loc) const {
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        return chunks.at(chunkID)->blockIsOpaque(getLocWithinChunk(loc));
    }
    return false;
}


bool ChunkManager::notAir(const LocInt& loc) const{
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        return chunks.at(chunkID)->notAir(getLocWithinChunk(loc));
    }
    return false;
}

void ChunkManager::calculateMeshesAsync(const LocInt& loc,int distance){
    ChunkID chunkId = getChunkID(loc);
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(int dx=-distance; dx<=distance; dx++){
        for(int dz=-distance; dz<=distance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            assert(chunks.count(chunkCandidate));
            if(chunks[chunkCandidate]->isGenerated()){
                if( chunks[chunkCandidate]->isDirty() && neighborsGenerated(chunkCandidate) && !chunks[chunkCandidate]->getCalculatingMeshFlag()){
                    // std::cout<< "Mesh creation asynchronously!" << std::endl;
                    chunks[chunkCandidate]->setCalculatingMeshFlagTrue();
                    Chunk* nbChunkNegX = chunks.at(chunkCandidate+nbDiffs[0]).get();
                    Chunk* nbChunkPosX = chunks.at(chunkCandidate+nbDiffs[1]).get();
                    Chunk* nbChunkNegZ = chunks.at(chunkCandidate+nbDiffs[2]).get();
                    Chunk* nbChunkPosZ = chunks.at(chunkCandidate+nbDiffs[3]).get();

                    futureDump.push_back(std::async(std::launch::async, &Chunk::update_mesh,chunks.at(chunkCandidate).get(),nbChunkNegX,nbChunkPosX,nbChunkNegZ,nbChunkPosZ));
                }
            }
        }
    }
}

void ChunkManager::calculateMeshesAsync(const LocInt& loc){
    calculateMeshesAsync(loc,renderDistance);
}

void ChunkManager::calculateMeshes(const LocInt& loc, int distance){
    ChunkID chunkId = getChunkID(loc);
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(int dx=-distance; dx<=distance; dx++){
        for(int dz=-distance; dz<=distance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            assert(chunks.count(chunkCandidate));
            if(chunks[chunkCandidate]->isGenerated()){
                if( chunks[chunkCandidate]->isDirty() && neighborsGenerated(chunkCandidate) && !chunks[chunkCandidate]->getCalculatingMeshFlag()){
                    // std::cout<< "Mesh creation in main thread!" << std::endl;
                    chunks[chunkCandidate]->setCalculatingMeshFlagTrue();
                    Chunk* nbChunkNegX = chunks.at(chunkCandidate+nbDiffs[0]).get();
                    Chunk* nbChunkPosX = chunks.at(chunkCandidate+nbDiffs[1]).get();
                    Chunk* nbChunkNegZ = chunks.at(chunkCandidate+nbDiffs[2]).get();
                    Chunk* nbChunkPosZ = chunks.at(chunkCandidate+nbDiffs[3]).get();
                    chunks[chunkCandidate]->update_mesh(nbChunkNegX,nbChunkPosX,nbChunkNegZ,nbChunkPosZ);
                }
            }
        }
    }
}

void ChunkManager::calculateMeshes(const LocInt& loc){
    calculateMeshes(loc,renderDistance);
}

std::queue<std::shared_ptr<RenderableChunkMesh>> ChunkManager::toRenderableChunkQueue( const LocInt& loc){
    ChunkID chunkId = getChunkID(loc);
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(int dx=-renderDistance; dx<=renderDistance; dx++){
        for(int dz=-renderDistance; dz<=renderDistance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            assert(chunks.count(chunkCandidate));
            if( !chunks[chunkCandidate]->isDirty() && !chunks[chunkCandidate]->getCalculatingMeshFlag()){
                chunkQueue.push(chunks[chunkCandidate]->getMeshPtr());
                // if(chunks[chunkCandidate]->getMeshPtr()->cutoutMesh.size()>0){
                //     std::cout << "Flowers in mesh in chunkmanager!" << std::endl;
                // }
            }
        }
    }
    return chunkQueue;
}

void ChunkManager::addChunk(const ChunkID& chunkID){
    chunks.try_emplace(chunkID, std::make_unique<Chunk>(chunkID, *this));
}
bool ChunkManager::neighborsGenerated(const ChunkID& chunkID){
    for( int i=0; i<4; i++){
        ChunkID nb = chunkID + nbDiffs[i];
        if( chunks.count(nb)==0 || !chunks[nb]->isGenerated()){
            return false;
        }
    }
    return chunks[chunkID]->isGenerated();
}