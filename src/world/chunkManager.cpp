#include <world/chunk.hpp>
#include <world/chunkManager.hpp>

constexpr Chunk::GenerationPars generationPars = {
    128,
    20,
    5 
};


ChunkManager::ChunkManager(unsigned int seed){
    noise = FastNoiseLite(seed);
    const int nr_chunks_side = 20;

    for(int x=-MAXCHUNKX*nr_chunks_side/2; x<MAXCHUNKX*nr_chunks_side/2; x+=MAXCHUNKX){
        for(int z=-nr_chunks_side/2*MAXCHUNKZ; z<nr_chunks_side/2*MAXCHUNKZ; z+=MAXCHUNKZ){
            addChunk({x,z});
        }
    }
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

void ChunkManager::deleteBlock(const LocInt& loc){
    for(int i = 0; i<6; i++){
        LocInt nb = loc + dirs[i];
        ChunkID nbChunkId = getChunkID(nb);
        chunks.at(nbChunkId)->setDirty();
    }
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        chunks.at(chunkID)->deleteBlock(getLocWithinChunk(loc));
    }
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

    // std::cout << "Inside isSolid!" << std::endl;

    // ChunkID chunkID = {loc.x- (loc.x%MAXCHUNKX),loc.z- (loc.z%MAXCHUNKZ)};
    ChunkID chunkID = getChunkID(loc);
    if(chunks.count(chunkID)!=0){
        return chunks.at(chunkID)->blockIsSolid(getLocWithinChunk(loc));
    }
    return false;

}


std::queue<std::shared_ptr<RenderableChunkMesh>> ChunkManager::toRenderableChunkQueue(){
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(auto& c : chunks){
        if(c.second->isDirty()){
            c.second->update_mesh();
        }
        chunkQueue.push(c.second->getMeshPtr());
    }
    return chunkQueue;
}

void ChunkManager::addChunk(const ChunkID& chunkID){
    chunks.try_emplace(chunkID, std::make_unique<Chunk>(noise,chunkID,generationPars, *this));

}