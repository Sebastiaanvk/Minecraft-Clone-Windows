#include "../include/camera.hpp"


Camera::Camera(){

}

bool Camera::init(){
    camera_speed = 2.5f;

    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    yaw = -90.0f;
    pitch = 0.0f;
    fov = 45.0f;
}

void Camera::rotate(float dx, float dy){

    const float sensitivity = 0.1f;
    float xoffset = dx * sensitivity;
    float yoffset = dy * sensitivity;

    yaw   += xoffset;
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
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

}
void Camera::move_right(float deltaTime){
    float delta = deltaTime * camera_speed;
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

}

glm::mat4 Camera::getViewMatrix(){
    glm::lookAt(cameraPos,cameraFront+cameraPos, cameraUp);
}

float Camera::getFov(){
    return fov;
}