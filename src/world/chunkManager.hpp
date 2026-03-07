#ifndef BLOCK_MANAGER_HPP
#define BLOCK_MANAGER_HPP

#include <blockRegistry.hpp>
#include <util/loc.h>
#include <unordered_map>
#include <memory>
#include <queue>
#include <chrono> // Just for measuring the time it takes to calculate the mesh of a chunk.

class Chunk; // Chunk and ChunkManager include each other.

class ChunkManager {
public:
    ChunkManager(unsigned int seed);
    void generateChunks(const LocInt& loc);
    BlockID checkBlock(const LocInt& loc) const;
    bool isSolid(const LocInt& loc) const;
    bool isOpaque(const LocInt& loc) const;
    void placeBlock(const LocInt& loc,const BlockID& blockId);
    void deleteBlock(const LocInt& loc);
    const Chunk& getChunkPointer(const ChunkID& chunkID);

    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue( const LocInt& loc);

private:
    ChunkID getChunkID(const LocInt& loc) const;
    LocInt getLocWithinChunk(const LocInt& loc) const;
    std::unordered_map<ChunkID, std::unique_ptr<Chunk>> chunks;
    FastNoiseLite noise;
    void addChunk(const ChunkID& chunkID);    

    int renderDistance = 10;
    int chunkGenerationDistance = renderDistance+1; // Needs to be higher than the renderDistance or the game breaks!

};

#endif //BLOCK_MANAGER_HPP