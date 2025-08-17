#ifndef MODEL_HPP
#define MODEL_HPP

// #include <world/block.hpp>
#include <world/chunk.hpp>
#include <render/renderable.hpp>
#include <util/loc.h>
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <external/FastNoiseLite.h>
#include <world/chunkManager.hpp>

class World {
public:
    // World();
    World(unsigned int seed);

    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue();

private:
    ChunkManager chunkManager;
    void addChunk(const ChunkID& chunkID);    
};

#endif //MODEL_HPP