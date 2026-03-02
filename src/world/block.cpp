#include <world/block.hpp>



Block::Block(BlockID type,int x,int y,int z)
    :type(type),x(x),y(y),z(z)
    {}

RenderableBlock Block::toRenderable(){
    RenderableBlock ren;
    ren.x=x;
    ren.y=y;
    ren.z=z;
    switch (type)
    {
    case BlockID::Dirt:
        ren.sideTexture = "dirt.png";
        ren.botTexture = "dirt.png";
        ren.topTexture = "dirt.png";
        break;
    case BlockID::Gravel:
        ren.sideTexture = "gravel.png";
        ren.botTexture = "gravel.png";
        ren.topTexture = "gravel.png";
        break;
    case BlockID::Stone:
        ren.sideTexture = "stone.png";
        ren.botTexture = "stone.png";
        ren.topTexture = "stone.png";
        break;
    case BlockID::Grass_Dirt:
        ren.sideTexture = "grass_block_side.png";
        ren.botTexture = "dirt.png";
        ren.topTexture = "green_concrete_powder.png";
        break;
    default:
        break;
    }
    return ren;
}