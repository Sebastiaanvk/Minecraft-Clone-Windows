#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <world/world.hpp>
#include <camera.hpp>
#include <memory>
#include <render/renderable.hpp>
#include <shaders/shader.h>
#include <util/loc.h>
#include <external/stb_image.h>
#include <external/json.hpp>
#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>
#include <external/glm/gtc/type_ptr.hpp>

class RenderMesh{
    public:
    unsigned int VAO;
    unsigned int VBO;
    GLsizei nrVertices;
};

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
    int atlasWidth;
    int atlasHeight;
    float textureSizeHeight;
    float textureSizeWidth;
    std::vector<float> updateVBOVector(const RenderableChunkMesh& worldMesh);
    RenderMesh createRenderMesh(const RenderableChunkMesh& worldMesh);
    std::map<ChunkID,RenderMesh> chunkMeshes;
};


#endif //RENDERER_HPP