#ifndef BLOCK_REGISTRY_HPP
#define BLOCK_REGISTRY_HPP

#include <array>


//typedef int BlockType;

enum BlockID{
    Air,
    Dirt,
    Gravel,
    Stone,
    Grass_Dirt,
    Bedrock,
    TYPE_COUNT
};


class BlockRegistry{
private:
    struct BlockData {
        bool solid;
    };
    static std::array<BlockData,BlockID::TYPE_COUNT> registry;

public:
    BlockRegistry() = delete;
    static void init();
    static bool is_solid(BlockID id);
};





#endif //BLOCK_REGISTRY_HPP