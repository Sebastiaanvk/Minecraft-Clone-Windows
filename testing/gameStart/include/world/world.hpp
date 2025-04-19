#ifndef MODEL_HPP
#define MODEL_HPP

#include <world/block.hpp>
#include <render/renderable.hpp>
#include <queue>
#include <vector>

class World{
public:
    World();

    std::queue<Renderable> toRenderableQueue();

private:
    std::vector<Block> blocks;
    
};

#endif //MODEL_HPP