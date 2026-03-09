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
    TYPE_COUNT
};


class BlockRegistry{
private:
    struct BlockData {
        bool solid;
        bool opaque;
        std::string_view textureNameSide;
        std::string_view textureNameTop;
        std::string_view textureNameBottom;
    };
    inline static constexpr std::array<BlockData,static_cast<std::size_t>(BlockID::TYPE_COUNT)> registry = {{
        //Air:
    {
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
        "dirt.png",
        "dirt.png",
        "dirt.png"
    },

        //Gravel:
{
        true,
        true,
        "gravel.png",
        "gravel.png",
        "gravel.png"
    },
        //Stone:
{
        true,
        true,
        "cobblestone.png",
        "cobblestone.png",
        "cobblestone.png"
    },
        //Grass_Dirt:
{
        true,
        true,
        "grass_block_side.png",
        "grass_block_top.png",
        "dirt.png"
    },
        //Bedrock:
{
        true,
        true,
        "bedrock.png",
        "bedrock.png",
        "bedrock.png"
    },
        //White whool:
{
        true,
        true,
        "white_wool.png",
        "white_wool.png",
        "white_wool.png"
    },
        //Black whool:
{
        true,
        true,
        "black_wool.png",
        "black_wool.png",
        "black_wool.png"
    },
        //Oak log:
{
        true,
        true,
        "oak_log.png",
        "oak_log_top.png",
        "oak_log_top.png"
    }
    }};

public:
    BlockRegistry() = delete;
    static bool is_solid(const BlockID& id);
    static bool isOpaque(const BlockID& id);
    static std::string getTextureName(const BlockID id, const FaceType faceType);
};





#endif //BLOCK_REGISTRY_HPP