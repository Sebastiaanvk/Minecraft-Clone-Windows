
#include <blockRegistry.hpp>


bool BlockRegistry::is_solid(BlockID id){
    return registry[id].solid;
}


std::string BlockRegistry::getTextureName(const BlockID id, const FaceType faceType){
    switch(faceType){
        case Side: return std::string(registry[id].textureNameSide);
        case Top: return std::string(registry[id].textureNameTop);
        default: return std::string(registry[id].textureNameBottom);
    }
}
