#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/world.hpp"
#include "../include/camera.hpp"

class Renderer {
public:
    Renderer();
    bool init(int width, int height);
    GLFWwindow* getWindow();
    void render(World& world, Camera& camera);
private:
    GLFWwindow* window;


};


#endif //RENDERER_HPP