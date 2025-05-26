#include <render/renderable.hpp>




class Block{
public:
    Block(BlockID type,int dx,int dy,int dz);
    RenderableBlock toRenderable();
private:
    int x,y,z;
    BlockID type;
};
