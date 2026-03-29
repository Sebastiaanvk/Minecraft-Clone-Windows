#include <world/player.hpp>

Player::Player()
    {
    // glm::vec3 direction;
    // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // direction.y = sin(glm::radians(pitch));
    // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // forward = glm::normalize(direction);
    forward = yawPitchToVector(yaw, pitch);
}

// void Player::storePos(){
//     lastPos = pos;
// }

void Player::rotate(float dx, float dy){

    float xoffset = dx * rotationSensitivity;
    float yoffset = dy * rotationSensitivity;

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

// void Player::move_forward(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     glm::vec3 horDir = glm::normalize(glm::vec3(forward.x,0,forward.z));
//     pos += delta*horDir;

// }

// void Player::move_backward(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     glm::vec3 horDir = glm::normalize(glm::vec3(forward.x,0,forward.z));
//     pos -= delta*horDir;

// }

// void Player::move_left(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     pos += glm::normalize(glm::cross(forward, UP)) * delta;

// }
// void Player::move_right(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     pos -= glm::normalize(glm::cross(forward, UP)) * delta;

// }

// void Player::move_up(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     pos += UP * delta;
// }

// void Player::move_down(float deltaTime){
//     float delta = deltaTime * playerSpeed;
//     pos -= UP * delta;
// }

LocFloat Player::getUpVelocity(float deltaTime) const{
    float delta = deltaTime * playerSpeed;
    return UP * delta;
}
LocFloat Player::getForwardVelocity(float deltaTime) const{
    float delta = deltaTime * playerSpeed;
    glm::vec3 horDir = glm::normalize(glm::vec3(forward.x,0,forward.z));
    return delta*horDir;
}
LocFloat Player::getRightVelocity(float deltaTime) const{
    float delta = deltaTime * playerSpeed;
    return glm::normalize(glm::cross(UP, forward)) * delta; // Double check whether this gives the right orientation.
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

void Player::setPos(const LocFloat& newPos ){
    lastPos = pos;
    pos = newPos;
}

LocFloat Player::getInterpolatedPos(float alpha) const{
    return alpha*pos + (1-alpha)*lastPos;
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
float Player::getYaw() const{
    return yaw;

}
float Player::getPitch() const{
    return pitch;
}

int Player::getHotbarSelectionIndex() const{
    return hotbarSelection;
}
void Player::changeHotbarSelection( bool moveUp){
    // Maybe switch these two.
    if(!moveUp){
        hotbarSelection = (hotbarSelection+1)%9;
    } else {
        hotbarSelection = (hotbarSelection-1+9)%9;
    }
}

PlayerUIData Player::getUIData(){
    PlayerUIData uiData;
    uiData.pos = pos;
    uiData.forwardDirection = forward;
    uiData.playerSpeedP = &playerSpeed;
    uiData.rotationSensitivityP = &rotationSensitivity;
    uiData.hotbarSelectionP = &hotbarSelection;

    return uiData;
}
bool Player::placeableBlockSelected(){
    // Change this later to check whether the block/item type is actually placeable.
    return hotbar[hotbarSelection].occupied;
}
BlockID Player::getSelectedBlock(){
    return hotbar[hotbarSelection].contents;
}
const std::array<InventorySlot,9>& Player::getHotbar() const{
    return hotbar;
}