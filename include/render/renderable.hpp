#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <external/glm/vec3.hpp>
#include <string>
#include <vector>
#include <blockRegistry.hpp>
#include <util/loc.h>


 

struct ChunkMeshElt{
    LocInt corners[4];
    BlockID  blockType;
    FaceType faceType;
    //Chatgpt suggested this datatype:
    uint8_t tint[3];
    uint8_t padding;
};

struct RenderableChunkMesh{
    ChunkID chunkId;
    std::vector<ChunkMeshElt> mesh;
    bool updated;
};

struct RenderableBlock{
    int x,y,z;
    std::string topTexture;
    std::string sideTexture;
    std::string botTexture;
};

#endif // RENDERABLE_HPP