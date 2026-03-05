#ifndef BLOCK_HPP
#define BLOCK_HPP


#include <render/renderable.hpp>
#include <blockRegistry.hpp>





class Block{
public:
    Block(BlockID type,int dx,int dy,int dz);
    RenderableBlock toRenderable();
private:
    int x,y,z;
    BlockID type;
};

#endif //BLOCK_HPP
