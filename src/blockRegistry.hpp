#ifndef BLOCK_REGISTRY_HPP
#define BLOCK_REGISTRY_HPP

#include <array>
#include <string>
#include <cstdint>

enum class FaceType : std::uint8_t {Side, Top, Bot};
inline const FaceType faceTypeArr[] = {FaceType::Side,FaceType::Side,FaceType::Top,FaceType::Bot,FaceType::Side,FaceType::Side};
inline const FaceType faceTypeArrIngoing[] = {FaceType::Side,FaceType::Side,FaceType::Bot,FaceType::Top,FaceType::Side,FaceType::Side};

enum class BlockID : std::uint8_t {
    Air,
    Dirt,
    Gravel,
    Stone,
    Grass_Dirt,
    Bedrock,
    White_Wool,
    Black_Wool,
    Oak_Log,
    Dandelion,
    Poppy,
    TYPE_COUNT
};


class BlockRegistry{
private:
    struct BlockData {
        bool solid;
        bool opaque;
        bool flower;
        std::string_view textureNameSide;
        std::string_view textureNameTop;
        std::string_view textureNameBottom;
    };
    inline static constexpr std::array<BlockData,static_cast<std::size_t>(BlockID::TYPE_COUNT)> registry = {{
        //Air:
    {
        false,
        false,
        false,
        "",
        "",
        ""
    },
        //Dirt:
    {
        true,
        true,
        false,
        "dirt.png",
        "dirt.png",
        "dirt.png"
    },

        //Gravel:
{
        true,
        true,
        false,
        "gravel.png",
        "gravel.png",
        "gravel.png"
    },
        //Stone:
{
        true,
        true,
        false,
        "cobblestone.png",
        "cobblestone.png",
        "cobblestone.png"
    },
        //Grass_Dirt:
{
        true,
        true,
        false,
        "grass_block_side.png",
        "grass_block_top.png",
        "dirt.png"
    },
        //Bedrock:
{
        true,
        true,
        false,
        "bedrock.png",
        "bedrock.png",
        "bedrock.png"
    },
        //White whool:
{
        true,
        true,
        false,
        "white_wool.png",
        "white_wool.png",
        "white_wool.png"
    },
        //Black whool:
{
        true,
        true,
        false,
        "black_wool.png",
        "black_wool.png",
        "black_wool.png"
    },
        //Oak log:
{
        true,
        true,
        false,
        "oak_log.png",
        "oak_log_top.png",
        "oak_log_top.png"
    },
        //Dandelion:
{
        false,
        false,
        true,
        "dandelion.png",
        "dandelion.png",
        "dandelion.png"
    },
        //Poppy:
{
        false,
        false,
        true,
        "poppy.png",
        "poppy.png",
        "poppy.png"
    }
    }};

public:
    BlockRegistry() = delete;
    static bool is_solid(const BlockID& id);
    static bool isOpaque(const BlockID& id);
    static bool isFlower(const BlockID& id);
    static std::string getTextureName(const BlockID id, const FaceType faceType);
};





#endif //BLOCK_REGISTRY_HPP