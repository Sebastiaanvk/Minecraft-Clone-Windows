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
            for(int y=0; y<200; y++){
                LocInt loc = { x,y,z};
                BlockID id = BlockID::Dirt;
                blockList.push_back({loc,id});
            }
        }
    }

    chunks[{0,0}] = Chunk(blockList,{0,0});
    chunks[{32,0}] = Chunk(blockList,{32,0});
    chunks[{-32,0}] = Chunk(blockList,{-32,0});



}


std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(auto& c : chunks){
        chunkQueue.push(c.second.getMeshPtr());
    }
    return chunkQueue;
}

// std::queue<RenderableBlock> World::toRenderableQueue(){
//     std::queue<RenderableBlock> blockQueue;
//     for(auto b : blocks){
//         blockQueue.push(b.toRenderable());
//     }
//     return blockQueue;
// }