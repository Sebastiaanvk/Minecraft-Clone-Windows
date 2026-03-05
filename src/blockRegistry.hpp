#ifndef BLOCK_REGISTRY_HPP
#define BLOCK_REGISTRY_HPP

#include <array>
#include <string>
#include <cstdint>

enum FaceType {Side, Top, Bot};
inline const FaceType faceTypeArr[] = {Side,Side,Top,Bot,Side,Side};

enum class BlockID : std::uint8_t {
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
        std::string_view textureNameSide;
        std::string_view textureNameTop;
        std::string_view textureNameBottom;
    };
    inline static constexpr std::array<BlockData,static_cast<std::size_t>(BlockID::TYPE_COUNT)> registry = {{
        //Air:
    {
        false,
        "",
        "",
        ""
    },
        //Dirt:
    {
        true,
        "dirt.png",
        "dirt.png",
        "dirt.png"
    },

        //Gravel:
{
        true,
        "gravel.png",
        "gravel.png",
        "gravel.png"
    },
        //Stone:
{
        true,
        "cobblestone.png",
        "cobblestone.png",
        "cobblestone.png"
    },
        //Grass_Dirt:
{
        true,
        "grass_block_side.png",
        "grass_block_top.png",
        "dirt.png"
    },
        //Bedrock:
{
        true,
        "bedrock.png",
        "bedrock.png",
        "bedrock.png"
    }

    }};

public:
    BlockRegistry() = delete;
    static bool is_solid(const BlockID& id);
    static std::string getTextureName(const BlockID id, const FaceType faceType);
};





#endif //BLOCK_REGISTRY_HPP