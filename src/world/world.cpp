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
    for(int x=6; x<9; x++){
        for(int z=6; z<9; z++){
            for(int y=2; y<6; y++){
                LocInt loc = { x,y,z};
                BlockID id = BlockID::Dirt;
                blockList.push_back({loc,id});
            }
        }
    }

    chunks[{0,0}] = Chunk(blockList,{0,0});
    chunks[{32,0}] = Chunk(blockList,{32,0});
    chunks[{-32,0}] = Chunk(blockList,{-32,0});

     testMeshPtr = std::make_shared<RenderableChunkMesh>();
    testMeshPtr->chunkId = {42,42};
    testMeshPtr->updated = true;
    ChunkMeshElt elt1 = {{{0,0,2},{2,0,2},{2,2,2},{0,2,2}},BlockID::Dirt,FaceType::Side};
    ChunkMeshElt elt2 = {{{2,0,2},{2,0,4},{2,2,4},{2,2,2}},BlockID::Bedrock,FaceType::Side};
    ChunkMeshElt elt3 = {{{2,0,4},{0,0,4},{0,2,4},{2,2,4}},BlockID::Gravel,FaceType::Side};
    ChunkMeshElt elt4 = {{{0,2,2},{2,2,2},{2,2,4},{0,2,4}},BlockID::Grass_Dirt,FaceType::Top};
    ChunkMeshElt elt5 = {{{0,0,4},{0,0,2},{0,2,2},{0,2,4}},BlockID::Grass_Dirt,FaceType::Side};
    ChunkMeshElt elt6 = {{{0,0,4},{2,0,4},{2,0,2},{0,0,2}},BlockID::Stone,FaceType::Side};
    testMeshPtr->mesh = { elt1,elt2,elt3,elt4,elt5,elt6};


}




std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(auto& c : chunks){
        std::cout << c.first.x << ", " << c.first.z << std::endl;
        if(c.second.isDirty()){
            c.second.update_mesh();
        }
        chunkQueue.push(c.second.getMeshPtr());
    }
    chunkQueue.push(testMeshPtr);
    return chunkQueue;
}

// std::queue<RenderableBlock> World::toRenderableQueue(){
//     std::queue<RenderableBlock> blockQueue;
//     for(auto b : blocks){
//         blockQueue.push(b.toRenderable());
//     }
//     return blockQueue;
// }