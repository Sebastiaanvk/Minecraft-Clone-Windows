#ifndef MODEL_HPP
#define MODEL_HPP

#include <world/block.hpp>
#include <world/chunk.hpp>
#include <render/renderable.hpp>
#include <util/loc.h>
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>

class World{
public:
    World();

    std::queue<RenderableBlock> toRenderableQueue();

private:
    std::vector<Block> blocks;
    std::unordered_map<Loc2, Chunk> chunks;
};

#endif //MODEL_HPP