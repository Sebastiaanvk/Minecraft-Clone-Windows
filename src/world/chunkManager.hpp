#ifndef BLOCK_MANAGER_HPP
#define BLOCK_MANAGER_HPP

#include <blockRegistry.hpp>
#include <world/chunkGeneration.hpp>
#include <util/loc.h>
#include <unordered_map>
#include <memory>
#include <queue>
#include <chrono> // Just for measuring the time it takes to calculate the mesh of a chunk.
#include <future>
#include <cassert>
#include <glm/glm.hpp>
#include <thread>
#include <BS_thread_pool.hpp>

class Chunk; // Chunk and ChunkManager include each other.

class ChunkManager {
public:
    ChunkManager(unsigned int seed);
    BlockID checkBlock(const LocInt& loc) const;
    bool isSolid(const LocInt& loc) const;
    bool isOpaque(const LocInt& loc) const;
    bool notAir(const LocInt& loc) const;
    bool isUnderwater(const LocInt& loc) const;
    void placeBlock(const LocInt& loc,const BlockID& blockId);
    void deleteBlock(const LocInt& loc);
    const Chunk& getChunkPointer(const ChunkID& chunkID);

    void generateTerrains(const LocInt& loc, int distance);
    void generateTerrains(const LocInt& loc);
    void generateTerrainsAsync(const LocInt& loc, int distance);
    void generateTerrainsAsync(const LocInt& loc);

    void generateTrees(const LocInt& loc, int distance);
    void generateTrees(const LocInt& loc);
    void generateTreesAsync(const LocInt& loc, int distance);
    void generateTreesAsync(const LocInt& loc);

    void calculateMeshesAsync(const LocInt& loc,int distance);
    void calculateMeshesAsync(const LocInt& loc);
    void calculateMeshes(const LocInt& loc,int distance);
    void calculateMeshes(const LocInt& loc);
    
    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue( const LocInt& loc);


    struct FrustumCullingPars{
        LocFloat cameraLoc;
        // float fovX;
        // float fovY;
        glm::vec3 forward;
        glm::vec3 normals[4];
        // glm::vec3 right;
        // glm::vec3 left;
        // glm::vec3 top;
        // glm::vec3 bottom;
        float near;
        float far;
    };
    void cullChunks(const FrustumCullingPars& cullingPars);

private:
    ChunkID getChunkID(const LocInt& loc) const;
    LocInt getLocWithinChunk(const LocInt& loc) const;
    std::unordered_map<ChunkID, std::unique_ptr<Chunk>> chunks;
    void addChunk(const ChunkID& chunkID);    

    int renderDistance = 50;
    int treeGenerationDistance = renderDistance+1;
    int chunkGenerationDistance = renderDistance+2; // Needs to be higher than the renderDistance or the game breaks!

    bool allowedToStartGeneratingTrees(const ChunkID& chunkID);
    bool allowedToStartCalculatingMesh(const ChunkID& chunkID);

    std::vector<std::future<void>> futureDump;

    FrustumCullingPars frustumCullingPars;
    std::vector<ChunkID> culledChunksVector;
    bool chunkInFrustum(const LocFloat& cornerLoc);

    BS::thread_pool<> pool;

};

#endif //BLOCK_MANAGER_HPP