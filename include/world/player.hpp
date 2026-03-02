#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <util/loc.h>
#include <render/uiData.hpp>

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

    PlayerUIData getUIData();

private:

    float playerSpeed = 10.0f;

    float yaw = 90.0f;
    float pitch = 0.0f;

    float rotationSensitivity = 0.1f;

    LocFloat pos = glm::vec3(0.0f,128.0f, 0.0f);
    LocFloat lastPos;
    LocFloat forward;

};





#endif //PLAYER_HPP