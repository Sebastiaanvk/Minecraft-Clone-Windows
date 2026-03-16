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
    uint8_t occlusion[4];
};

struct CutoutMeshElt{
    LocFloat corners[4];
    BlockID blockType;
    uint8_t tint[3];
};

struct TranslucentMeshElt{
    LocFloat corners[4];
    BlockID blockType;
    FaceType faceType;
    uint8_t tint[3];
};

struct SolidVBOElt{
    glm::vec3 pos;
    glm::vec3 uvl;
    // int textureLayer;
    uint8_t tint[4];
    uint8_t occlusion;
};

struct CutoutVBOElt{
    glm::vec3 pos;
    glm::vec3 uvl;
    // int textureLayer;
    uint8_t tint[4];
};

struct TranslucentVBOElt{
    glm::vec3 pos;
    glm::vec3 uvl;
    // int textureLayer;
    uint8_t tint[4];
};
// One chunk mesh as a vector of sides of blocks
struct RenderableChunkMesh{
    ChunkID chunkId;
    std::vector<SolidVBOElt> solidMesh;
    std::vector<CutoutVBOElt> cutoutMesh;
    std::vector<TranslucentVBOElt> translucentMesh;
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