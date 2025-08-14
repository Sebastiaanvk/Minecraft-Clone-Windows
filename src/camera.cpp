#include <camera.hpp>


Camera::Camera(){

}

bool Camera::init(){
    camera_speed = 2.5f;

    cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    yaw = 90.0f;
    pitch = 0.0f;
    fov = 45.0f;

    return true;
}

void Camera::rotate(float dx, float dy){

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
    cameraFront = glm::normalize(direction);
}

void Camera::move_forward(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos += delta*cameraFront;

}

void Camera::move_backward(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos -= delta*cameraFront;

}

void Camera::move_left(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

}
void Camera::move_right(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

}

void Camera::move_up(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos += glm::vec3(0.0f,1.0f,0.0f) * delta;
}

void Camera::move_down(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos -= glm::vec3(0.0f,1.0f,0.0f) * delta;
}
std::string Camera::cameraStatsAsString(){
    std::string statsStr = "Camera stats: ";
    statsStr += "Camera position: "+ std::to_string(cameraPos.x) +  ", " + std::to_string(cameraPos.y) + ", "+ std::to_string(cameraPos.z); 
    statsStr += ", camera direction: "+ std::to_string(cameraFront.x) +  ", " + std::to_string(cameraFront.y) + ", "+ std::to_string(cameraFront.z) ; 

    return statsStr ;
}


glm::mat4 Camera::getViewMatrix(){
    // We flip the z, because in our world, x is to the right, z is forward and y is up
    // However, in opengl, the z axis goes out of the screen towards you.
    glm::vec3 cameraPosZFlip = glm::vec3(cameraPos.x,cameraPos.y,-cameraPos.z);
    glm::vec3 cameraFrontZFlip = glm::vec3(cameraFront.x,cameraFront.y,-cameraFront.z);
    glm::vec3 cameraUpZFlip = glm::vec3(cameraUp.x,cameraUp.y,-cameraUp.z);

    glm::mat4 view = glm::lookAt(cameraPosZFlip,cameraFrontZFlip+cameraPosZFlip, cameraUpZFlip);

    return view;
}

float Camera::getFov(){
    return fov;
}