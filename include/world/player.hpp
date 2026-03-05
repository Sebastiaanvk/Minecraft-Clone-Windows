#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <util/loc.h>
#include <render/uiData.hpp>
#include <blockRegistry.hpp>

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

private:

    float playerSpeed = 20.0f;

    float yaw = 90.0f;
    float pitch = 0.0f;

    float rotationSensitivity = 0.1f;

    int hotbarSelection = 0;

    LocFloat pos = glm::vec3(0.0f,128.0f, 0.0f);
    LocFloat lastPos;
    LocFloat forward;
    struct InventorySlot {
        bool occupied;
        BlockID blockId;
        
    };

    InventorySlot hotbar[9] = {
        {true,BlockID::Dirt},
        {true,BlockID::Gravel},
        {true,BlockID::Stone},
        {true,BlockID::Bedrock},
        {false,BlockID::Air},
        {false,BlockID::Air},
        {false,BlockID::Air},
        {false,BlockID::Air},
        {false,BlockID::Air}
    };
};





#endif //PLAYER_HPP