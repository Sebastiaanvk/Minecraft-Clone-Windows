#ifndef BLOCK_MANAGER_HPP
#define BLOCK_MANAGER_HPP

#include <blockRegistry.hpp>
#include <util/loc.h>
#include <unordered_map>
#include <memory>
#include <queue>

class Chunk;

class ChunkManager {
public:
    // ChunkManager();
    ChunkManager(unsigned int seed);
    BlockID checkBlock(const LocInt& loc) const;
    bool isSolid(const LocInt& loc) const;

    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue();

private:
    ChunkID getChunkID(const LocInt& loc) const;
    LocInt getLocWithinChunk(const LocInt& loc) const;
    std::unordered_map<ChunkID, std::unique_ptr<Chunk>> chunks;
    FastNoiseLite noise;
    void addChunk(const ChunkID& chunkID);    
};

#endif //BLOCK_MANAGER_HPP