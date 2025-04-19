#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../include/input_handler.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    Camera();
    bool init();
    void rotate(float dx, float dy);
    void move_forward(float deltaTime);
    void move_backward(float deltaTime);
    void move_left(float deltaTime);
    void move_right(float deltaTime);

    glm::mat4 getViewMatrix();

    float getFov();
private:

    float camera_speed;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float fov;
    
};


#endif #CAMERA_HPP