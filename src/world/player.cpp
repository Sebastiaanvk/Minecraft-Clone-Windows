#include <world/player.hpp>

Player::Player()
    :playerSpeed(10.0f),
    pos(glm::vec3(0.0f, 128.0f,  0.0f)),
    yaw(90.0f),
    pitch(0.0f){
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(direction);

}

void Player::rotate(float dx, float dy){

    const float sensitivity = 0.1f;
    float xoffset = dx * sensitivity;
    float yoffset = dy * sensitivity;

    yaw   -= xoffset;
    pitch += yoffset;  

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(direction);
}

void Player::move_forward(float deltaTime){
    float delta = deltaTime * playerSpeed;
    glm::vec3 horDir = glm::normalize(glm::vec3(forward.x,0,forward.z));
    pos += delta*horDir;

}

void Player::move_backward(float deltaTime){
    float delta = deltaTime * playerSpeed;
    glm::vec3 horDir = glm::normalize(glm::vec3(forward.x,0,forward.z));
    pos -= delta*horDir;

}

void Player::move_left(float deltaTime){
    float delta = deltaTime * playerSpeed;
    pos += glm::normalize(glm::cross(forward, UP)) * delta;

}
void Player::move_right(float deltaTime){
    float delta = deltaTime * playerSpeed;
    pos -= glm::normalize(glm::cross(forward, UP)) * delta;

}

void Player::move_up(float deltaTime){
    float delta = deltaTime * playerSpeed;
    pos += UP * delta;
}

void Player::move_down(float deltaTime){
    float delta = deltaTime * playerSpeed;
    pos -= UP * delta;
}
std::string Player::playerLocAsString(){
    std::string statsStr = "Player stats: ";
    statsStr += "Player position: "+ std::to_string(pos.x) +  ", " + std::to_string(pos.y) + ", "+ std::to_string(pos.z); 
    statsStr += ", player forward direction: "+ std::to_string(forward.x) +  ", " + std::to_string(forward.y) + ", "+ std::to_string(forward.z) ; 

    return statsStr ;
}

LocFloat Player::getPos() const{
    return pos;
}

LocInt Player::getBlockLoc() const{
    return posToBlockLoc(pos);
}

bool Player::blockIntersects(const LocInt& loc ){
    return loc==getBlockLoc() || loc+LocInt({0,1,0})==getBlockLoc();
}

LocFloat Player::getForwardDir() const{
    return forward;
}