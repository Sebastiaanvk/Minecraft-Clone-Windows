#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <blockRegistry.hpp>
#include <util/loc.h>

// Im adding renderable to a lot of the structs in this file
// Maybe I turn it into a namespace at some point.
 
// A single side of a block
struct ChunkMeshElt{
    LocInt corners[4];
    BlockID  blockType;
    FaceType faceType;
    //Chatgpt suggested this datatype:
    uint8_t tint[3];
    uint8_t padding; // This is there to give the struct a round number of bytes. Probably not necessary anymore!
};

struct CutoutMeshElt{
    LocFloat corners[4];
    BlockID blockType;
    // uint8_t tint[3];
};

// One chunk mesh as a vector of sides of blocks
struct RenderableChunkMesh{
    ChunkID chunkId;
    std::vector<ChunkMeshElt> solidMesh;
    std::vector<CutoutMeshElt> cutoutMesh;
    bool updated;
};

//For rendering a single block
struct RenderableBlock{
    int x,y,z;
    std::string topTexture;
    std::string sideTexture;
    std::string botTexture;
};

struct RenderableInventory{
    BlockID slotContents[9];
    bool slotOccupied[9];
};

#endif // RENDERABLE_HPP