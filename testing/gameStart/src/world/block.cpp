#include <world/block.hpp>



Block::Block(BlockType type,int x,int y,int z)
    :type(type),x(x),y(y),z(z)
    {}

Renderable Block::toRenderable(){
    Renderable ren;
    ren.x=x;
    ren.y=y;
    ren.z=z;
    switch (type)
    {
    case DIRT:
        ren.sideTexture = "dirt.png";
        ren.botTexture = "dirt.png";
        ren.topTexture = "dirt.png";
        break;
    case GRAVEL:
        ren.sideTexture = "gravel.png";
        ren.botTexture = "gravel.png";
        ren.topTexture = "gravel.png";
        break;
    case STONE:
        ren.sideTexture = "stone.png";
        ren.botTexture = "stone.png";
        ren.topTexture = "stone.png";
        break;
    case GRASS_DIRT:
        ren.sideTexture = "grass_block_side.png";
        ren.botTexture = "dirt.png";
        ren.topTexture = "green_concrete_powder.png";
        break;
    default:
        break;
    }
    return ren;
}