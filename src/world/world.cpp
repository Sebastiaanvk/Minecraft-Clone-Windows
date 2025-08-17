#include <world/world.hpp>



// World::World(){

// }

World::World(unsigned int seed)
: chunkManager(seed){
}



std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    return chunkManager.toRenderableChunkQueue();
}




