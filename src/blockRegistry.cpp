
#include <blockRegistry.hpp>


bool BlockRegistry::is_solid(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].solid;
}

bool BlockRegistry::isOpaque(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].opaque;
}
bool BlockRegistry::isTranslucent(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].translucent;
}

bool BlockRegistry::isCross(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].cross;
}

bool BlockRegistry::isUnderwater(const BlockID& id){
    return registry[static_cast<std::size_t>(id)].underwater;
}

std::string BlockRegistry::getTextureName(const BlockID id, const FaceType faceType){
    switch(faceType){
        case FaceType::Side: return std::string(registry[static_cast<std::size_t>(id)].textureNameSide);
        case FaceType::Top: return std::string(registry[static_cast<std::size_t>(id)].textureNameTop);
        default: return std::string(registry[static_cast<std::size_t>(id)].textureNameBottom);
    }
}

int BlockRegistry::getTextureIndex(const BlockID id, const FaceType faceType){
    return (static_cast<int>(id)-1)*3+static_cast<int>(faceType);
}
int BlockRegistry::nrTextureIndices(){
    return (static_cast<int>(BlockID::TYPE_COUNT)-1) * 3;
}

std::string BlockRegistry::indexToTextureName(int index){
    if(index%3==0){
        return std::string(registry[(index/3)+1].textureNameSide);
    } else if(index%3==1){
        return std::string(registry[(index/3)+1].textureNameTop);
    }
    return std::string(registry[(index/3)+1].textureNameBottom);
}
