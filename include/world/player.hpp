#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <util/loc.h>

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
private:

    float playerSpeed;

    float yaw;
    float pitch;

    LocFloat pos;
    LocFloat lastPos;
    LocFloat forward;

};





#endif //PLAYER_HPP