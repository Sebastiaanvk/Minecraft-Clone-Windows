#include <world/world.hpp>



World::World(){
    blocks = {};
    for(int x=-20; x<20; x++){
        for(int z=-20; z<20; z++){
            blocks.push_back(Block(BlockType::DIRT,x,0,z));
        }
    }
}

std::queue<Renderable> World::toRenderableQueue(){
    std::queue<Renderable> blockQueue;
    for(auto b : blocks){
        blockQueue.push(b.toRenderable());
    }
    return blockQueue;
}