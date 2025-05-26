#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <world/world.hpp>
#include <camera.hpp>
#include <render/renderable.hpp>
#include <shaders/shader.h>
#include <external/stb_image.h>
#include <external/json.hpp>
#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>
#include <external/glm/gtc/type_ptr.hpp>


class Renderer {
public:
    Renderer();

    bool init(int width, int height);

    GLFWwindow* getWindow();

    void render(World& world, Camera& camera);
private:
    GLFWwindow* window;
    nlohmann::json jsonAtlasData;
    Shader shaderprogram;
    void setUpBuffers();
    unsigned int VAO; // I will change this, this is just to get it to work quickly.
    unsigned int VBO;
    unsigned int VBO2;
    int atlasWidth;
    int atlasHeight;
};


#endif //RENDERER_HPP