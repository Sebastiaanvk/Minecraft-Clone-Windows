#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <blockRegistry.hpp>
#include <util/loc.h>


 
// A single side of a block
struct ChunkMeshElt{
    LocInt corners[4];
    BlockID  blockType;
    FaceType faceType;
    //Chatgpt suggested this datatype:
    uint8_t tint[3];
    uint8_t padding;
};

// One chunk mesh as a vector of sides of blocks
struct RenderableChunkMesh{
    ChunkID chunkId;
    std::vector<ChunkMeshElt> mesh;
    bool updated;
};

//For rendering a single block
struct RenderableBlock{
    int x,y,z;
    std::string topTexture;
    std::string sideTexture;
    std::string botTexture;
};

#endif // RENDERABLE_HPP