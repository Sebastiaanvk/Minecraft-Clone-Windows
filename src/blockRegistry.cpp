
#include <blockRegistry.hpp>




void BlockRegistry::init(){
    registry[BlockID::Air] = {
        false
    };
    registry[BlockID::Dirt] = {
        true
    };
    registry[BlockID::Gravel] = {
        true
    };
    registry[BlockID::Stone] = {
        true
    };
    registry[BlockID::Grass_Dirt] = {
        true
    };
    registry[BlockID::Bedrock] = {
        true
    };
}

bool BlockRegistry::is_solid(BlockID id){
    return registry[id].solid;
}


