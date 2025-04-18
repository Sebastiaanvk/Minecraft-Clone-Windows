#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer();
    bool init(int width, int height);
    GLFWwindow* getWindow();
    void render();
private:
    GLFWwindow* window;


};


#endif //RENDERER_HPP