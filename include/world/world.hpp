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

class World{
public:
    World();

    // std::queue<RenderableBlock> toRenderableQueue();
    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue();

private:
    std::vector<Block> blocks;
    std::unordered_map<ChunkID, Chunk> chunks;
};

#endif //MODEL_HPP