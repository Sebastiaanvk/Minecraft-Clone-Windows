
#include <blockRegistry.hpp>


bool BlockRegistry::is_solid(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].solid;
}

bool BlockRegistry::isOpaque(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].opaque;
}

std::string BlockRegistry::getTextureName(const BlockID id, const FaceType faceType){
    switch(faceType){
        case FaceType::Side: return std::string(registry[static_cast<std::size_t>(id)].textureNameSide);
        case FaceType::Top: return std::string(registry[static_cast<std::size_t>(id)].textureNameTop);
        default: return std::string(registry[static_cast<std::size_t>(id)].textureNameBottom);
    }
}
