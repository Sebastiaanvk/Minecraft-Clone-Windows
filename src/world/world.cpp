#include <world/world.hpp>


constexpr Chunk::GenerationPars generationPars = {
    128,
    20,
    5 
};



World::World(unsigned int seed){
    FastNoiseLite noise = FastNoiseLite(seed);

    const int nr_chunks_side = 20;

    for(int x=-MAXCHUNKX*nr_chunks_side/2; x<MAXCHUNKX*nr_chunks_side/2; x+=MAXCHUNKX){
        for(int z=-nr_chunks_side/2*MAXCHUNKZ; z<nr_chunks_side/2*MAXCHUNKZ; z+=MAXCHUNKZ){
            // chunks[{x,z}] = Chunk(blockList,{x,z}) ;
            chunks[{x,z}] = Chunk(noise,{x,z},generationPars);
        }
    }



}

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

    std::vector<std::pair<LocInt, BlockID>> blockList3;
    for(int x=6; x<9; x++){
        for(int z=6; z<9; z++){
            for(int y=2; y<6; y++){
                LocInt loc = { x,y,z};
                BlockID id = BlockID::Grass_Dirt;
                blockList3.push_back({loc,id});
            }
        }
    }

    chunks[{16,0}] = Chunk(blockList,{32,0});
    chunks[{-32,0}] = Chunk(blockList,{-32,0});
    chunks[{0,16}] = Chunk(blockList3,{0,16});

    std::vector<std::pair<LocInt, BlockID>> blockList2;
    blockList2.push_back({{0,0,5},BlockID::Grass_Dirt});
    blockList2.push_back({{1,1,7},BlockID::Dirt});
    blockList2.push_back({{2,2,9},BlockID::Gravel});
    blockList2.push_back({{3,3,11},BlockID::Stone});
    blockList2.push_back({{4,4,13},BlockID::Bedrock});
    chunks[{0,0}] = Chunk(blockList2,{0,0});



}




std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue;
    for(auto& c : chunks){
        if(c.second.isDirty()){
            c.second.update_mesh();
        }
        chunkQueue.push(c.second.getMeshPtr());
    }
    return chunkQueue;
}
