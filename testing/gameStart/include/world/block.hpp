#include <render/renderable.hpp>


enum BlockType{
    DIRT,
    GRAVEL,
    STONE,
    GRASS_DIRT,
    TYPE_COUNT
};


class Block{
public:
    Block(BlockType type,int dx,int dy,int dz);
    Renderable toRenderable();
private:
    int x,y,z;
    BlockType type;
};
