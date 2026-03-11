#ifndef BLOCK_MANAGER_HPP
#define BLOCK_MANAGER_HPP

#include <blockRegistry.hpp>
#include <util/loc.h>
#include <unordered_map>
#include <memory>
#include <queue>
#include <chrono> // Just for measuring the time it takes to calculate the mesh of a chunk.
#include <future>
#include <cassert>

class Chunk; // Chunk and ChunkManager include each other.

class ChunkManager {
public:
    ChunkManager(unsigned int seed);
    void generateChunks(const LocInt& loc, int distance);
    void generateChunksAsync(const LocInt& loc, int distance);
    void generateChunks(const LocInt& loc);
    void generateChunksAsync(const LocInt& loc);
    BlockID checkBlock(const LocInt& loc) const;
    bool isSolid(const LocInt& loc) const;
    bool isOpaque(const LocInt& loc) const;
    bool notAir(const LocInt& loc) const;
    void placeBlock(const LocInt& loc,const BlockID& blockId);
    void deleteBlock(const LocInt& loc);
    const Chunk& getChunkPointer(const ChunkID& chunkID);


    void calculateMeshesAsync(const LocInt& loc,int distance);
    void calculateMeshesAsync(const LocInt& loc);
    void calculateMeshes(const LocInt& loc,int distance);
    void calculateMeshes(const LocInt& loc);
    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue( const LocInt& loc);

    bool neighborsGenerated(const ChunkID& chunkID);

private:
    ChunkID getChunkID(const LocInt& loc) const;
    LocInt getLocWithinChunk(const LocInt& loc) const;
    std::unordered_map<ChunkID, std::unique_ptr<Chunk>> chunks;
    FastNoiseLite noise;
    void addChunk(const ChunkID& chunkID);    

    int renderDistance = 20;
    int chunkGenerationDistance = renderDistance+2; // Needs to be higher than the renderDistance or the game breaks!

    std::vector<std::future<void>> futureDump;

};

#endif //BLOCK_MANAGER_HPP