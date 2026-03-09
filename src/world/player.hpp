#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <util/loc.h>
#include <render/uiData.hpp>
#include <blockRegistry.hpp>
#include <array>

struct InventorySlot {
    bool occupied;
    BlockID contents;
    // Add quantity when the time is ripe.
};

class Player{
public:
    Player();
    void storePos();
    void rotate(float dx, float dy);
    void move_forward(float deltaTime);
    void move_backward(float deltaTime);
    void move_left(float deltaTime);
    void move_right(float deltaTime);
    void move_up(float deltatime);
    void move_down(float deltatime);
    std::string playerLocAsString();

    LocFloat getPos() const;
    LocFloat getInterpolatedPos(float alpha) const;
    LocInt getBlockLoc() const;
    bool blockIntersects(const LocInt& loc );
    LocFloat getForwardDir() const;
    int getHotbarSelectionIndex() const;
    void changeHotbarSelection( bool moveUp);

    PlayerUIData getUIData();
    bool placeableBlockSelected();
    BlockID getSelectedBlock();

    const std::array<InventorySlot,9>& getHotbar() const;

private:

    float playerSpeed = 20.0f;

    float yaw = 90.0f;
    float pitch = 0.0f;

    float rotationSensitivity = 0.1f;

    int hotbarSelection = 0;

    // LocFloat pos = glm::vec3(0.0f,128.0f, 0.0f);
    LocFloat pos = glm::vec3(0.0f,200.0f, 0.0f);
    LocFloat lastPos;
    LocFloat forward;

    std::array<InventorySlot,9> hotbar = {{
        {true,BlockID::Dirt},
        {true,BlockID::Grass_Dirt},
        {true,BlockID::Gravel},
        {true,BlockID::Stone},
        {true,BlockID::Bedrock},
        {true,BlockID::White_Wool},
        {true,BlockID::Black_Wool},
        {false,BlockID::Air},
        {false,BlockID::Air}
    }};
};





#endif //PLAYER_HPP