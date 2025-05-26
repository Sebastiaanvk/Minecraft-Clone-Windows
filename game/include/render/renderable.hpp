#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <external/glm/vec3.hpp>
#include <string>
#include <blockRegistry.hpp>

enum FaceType  {Side,Side,Top,Bot,Side,Side};

struct ChunkMeshElt{
    LocInt corner0;
    LocInt corner1;
    LocInt corner2;
    LocInt corner3;
    BlockID  blockType;
    FaceType faceType;
};

struct RenderableChunkMesh{
    std::vector<ChunkMeshElt> mesh;
};

struct RenderableBlock{
    int x,y,z;
    std::string topTexture;
    std::string sideTexture;
    std::string botTexture;
};

#endif // RENDERABLE_HPP