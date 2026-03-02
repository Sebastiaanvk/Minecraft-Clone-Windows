#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <input/input_handler.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <world/player.hpp>
#include <render/uiData.hpp>

class Camera{
public:
    Camera();
    void update(const Player& player,float alpha);

    glm::mat4 getViewMatrix();

    float getFov();
    CameraUIData getUIData();
private:

    glm::mat4 view;
    float fov = 45.0f;

    LocFloat flipZ(const LocFloat& loc) const;
};


#endif //CAMERA_HPP