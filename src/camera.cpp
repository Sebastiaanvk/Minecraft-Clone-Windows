#include <camera.hpp>


Camera::Camera()
    :fov(45.0f)
{

}

void Camera::update(const Player& player,float alpha){
    // We flip the z, because in our world, x is to the right, z is forward and y is up
    // However, in opengl, the z axis goes out of the screen towards you.
    glm::vec3 cameraPosZFlip = flipZ(player.getInterpolatedPos(alpha));
    glm::vec3 cameraFrontZFlip = flipZ(player.getForwardDir());

    view = glm::lookAt(cameraPosZFlip,cameraFrontZFlip+cameraPosZFlip, UP);
}

glm::mat4 Camera::getViewMatrix(){
    return view;
}

float Camera::getFov(){
    return fov;
}

LocFloat Camera::flipZ(const LocFloat& loc) const{
    return LocFloat(loc.x,loc.y,-loc.z);
}

CameraUIData Camera::getUIData(){
    CameraUIData data;
    data.fovP = &fov;
    return data;
}