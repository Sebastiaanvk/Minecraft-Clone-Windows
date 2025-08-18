#include <world/world.hpp>



// World::World(){

// }

World::World(unsigned int seed)
: chunkManager(seed),player(){
}



std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    return chunkManager.toRenderableChunkQueue();
}




