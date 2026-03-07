#include <world/chunk.hpp> // Chunk and ChunkManager include each other.
#include <world/chunkManager.hpp>

constexpr Chunk::GenerationPars generationPars = {
    128,
    20,
    5 
};


void ChunkManager::generateChunks(const LocInt& loc){
    ChunkID chunkId = getChunkID(loc);
    for(int dx=-chunkGenerationDistance; dx<=chunkGenerationDistance; dx++){
        for(int dz=-chunkGenerationDistance; dz<=chunkGenerationDistance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            if(chunks.count(chunkCandidate)==0){
                addChunk(chunkCandidate);
            }
        }
    }
}

ChunkManager::ChunkManager(unsigned int seed){
    noise = FastNoiseLite(seed);
    generateChunks({0,0});
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

std::queue<std::shared_ptr<RenderableChunkMesh>> ChunkManager::toRenderableChunkQueue( const LocInt& loc){
    ChunkID chunkId = getChunkID(loc);
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(int dx=-renderDistance; dx<=renderDistance; dx++){
        for(int dz=-renderDistance; dz<=renderDistance; dz++){
            ChunkID chunkCandidate = {chunkId.x+dx*MAXCHUNKX,chunkId.z+dz*MAXCHUNKZ};
            if( chunks[chunkCandidate]->isDirty() ){
                std::cout << "Updating mesh:\n";
                auto start = std::chrono::high_resolution_clock::now();
                chunks[chunkCandidate]->update_mesh();
                auto end = std::chrono::high_resolution_clock::now();
                auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "Took: " << ms << "microseconds" << std::endl;
            }
            chunkQueue.push(chunks[chunkCandidate]->getMeshPtr());
        }
    }
    return chunkQueue;
}

void ChunkManager::addChunk(const ChunkID& chunkID){
    chunks.try_emplace(chunkID, std::make_unique<Chunk>(noise,chunkID,generationPars, *this));
}