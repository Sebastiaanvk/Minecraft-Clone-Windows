#ifndef BLOCK_REGISTRY_HPP
#define BLOCK_REGISTRY_HPP

#include <array>
#include <string>
#include <cstdint>

enum class FaceType : std::uint8_t {Side, Top, Bot};
inline const FaceType faceTypeArr[] = {FaceType::Side,FaceType::Side,FaceType::Top,FaceType::Bot,FaceType::Side,FaceType::Side};
inline const FaceType faceTypeArrIngoing[] = {FaceType::Side,FaceType::Side,FaceType::Bot,FaceType::Top,FaceType::Side,FaceType::Side};

// I will formalize this later on:
inline const uint8_t waterTint[3] = {0,128,190};
inline const uint8_t underwaterTint[3] = {0,128,200};
inline const uint8_t leavesTint[3] = {140,210,70};
inline const uint8_t grassTint[3] = {140,210,70};

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
    Oak_Leaves,
    Dandelion,
    Poppy,
    Short_Grass,
    Water,
    Seagrass,
    TubeCoral,
    BrainCoral,
    TYPE_COUNT
};


class BlockRegistry{
private:
    struct BlockData {
        bool solid; // Is this the same as being able to place a box next to it?
        bool opaque;
        bool translucent;
        bool cross;
        bool underwater;
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
        false,
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
        false,
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
        false,
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
        false,
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
        false,
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
        false,
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
        false,
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
        false,
        false,
        "oak_log.png",
        "oak_log_top.png",
        "oak_log_top.png"
    },
        //Oak leaves:
{
        true,
        false,
        false,
        false,
        false,
        "oak_leaves.png",
        "oak_leaves.png",
        "oak_leaves.png"
    },
        //Dandelion:
{
        false,
        false,
        false,
        true,
        false,
        "dandelion.png",
        "dandelion.png",
        "dandelion.png"
    },
        //Poppy:
{
        false, // solid
        false, // opaque
        false, // translucent
        true, // cross
        false, // underwater
        "poppy.png", // texture name side
        "poppy.png", // texture name top
        "poppy.png" // texture name bottom
    },
        //Short Grass:
{
        false, // solid
        false, // opaque
        false, // translucent
        true, // cross
        false, // underwater
        "short_grass.png", // texture name side
        "short_grass.png", // texture name top
        "short_grass.png" // texture name bottom
    },
        //Water:
{
        false, // solid
        false, // opaque
        true, // translucent
        false, // cross
        true, // underwater
        "water_overlay.png", // texture name side
        "water_still.png", // texture name top
        "water_overlay.png" // texture name bottom
    },
        //Seagrass:
{
        false, // solid
        false, // opaque
        false, // translucent
        true, // cross
        true, // underwater
        "seagrass.png", // texture name side
        "seagrass.png", // texture name top
        "seagrass.png" // texture name bottom
    },
        //Tube Coral:
{
        false, // solid
        false, // opaque
        false, // translucent
        true, // cross
        true, // underwater
        "tube_coral.png", // texture name side
        "tube_coral.png", // texture name top
        "tube_coral.png" // texture name bottom
    },
        //Brain Coral:
{
        false, // solid
        false, // opaque
        false, // translucent
        true, // cross
        true, // underwater
        "brain_coral.png", // texture name side
        "brain_coral.png", // texture name top
        "brain_coral.png" // texture name bottom
    }
    }};

public:
    BlockRegistry() = delete;
    static bool is_solid(const BlockID& id);
    static bool isOpaque(const BlockID& id);
    static bool isTranslucent(const BlockID& id);
    static bool isCross(const BlockID& id);
    static bool isUnderwater(const BlockID& id);
    static std::string getTextureName(const BlockID id, const FaceType faceType);
};





#endif //BLOCK_REGISTRY_HPP