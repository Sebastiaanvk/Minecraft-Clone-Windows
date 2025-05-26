#include <world/world.hpp>


/*
World::World(){
    blocks = {};
    for(int x=-20; x<20; x++){
        for(int z=-20; z<20; z++){
            blocks.push_back(Block(BlockType::Dirt,x,0,z));
        }
    }
}
*/

World::World(){
    blocks = {};
    chunks = {};
    std::vector<std::pair<LocInt, BlockID>> blockList;
    for(int x=0; x<40; x++){
        for(int z=0; z<40; z++){
            for(int y=50; y<150; y++){
                LocInt loc = { x,0,z};
                BlockID id = BlockID::Dirt;
                blockList.push_back({loc,id});
            }
        }
    }

    chunks[{0,0}] = Chunk(blockList);
    chunks[{2,0}] = Chunk(blockList);


}

std::queue<RenderableBlock> World::toRenderableQueue(){
    std::queue<RenderableBlock> blockQueue;
    for(auto b : blocks){
        blockQueue.push(b.toRenderable());
    }
    return blockQueue;
}