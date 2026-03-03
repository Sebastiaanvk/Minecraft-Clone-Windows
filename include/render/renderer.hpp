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
#include <render/uiData.hpp>
#include <shaders/shader.h>
#include <util/loc.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/customImGui.hpp>
#include <render/textures.hpp>

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

    void render(World& world, Camera& camera, GameUIData gameData);
    void shutDown();
private:
// Chatgpt suggested using a struct for the data of the vbo.
    struct chunkVBOElt{
        glm::vec3 pos;
        glm::vec2 uv;
        uint8_t tint[4];
    };

    GLFWwindow* window;
    Shader chunkShaderProgram;

    unsigned int VAOBlockOutline;
    unsigned int VBOBlockOutline;
    unsigned int EBOBlockOutline;
    Shader outLineShaderProgram;
    float localOutlineOffset = 0.002f;
    float localOutlineWidth = 7.0f;


     
    bool showGameData = true;
    float textureMargin = 0.002f;

    std::vector<chunkVBOElt> updateVBOVector(const RenderableChunkMesh& worldMesh);
    RenderMesh createRenderMesh(const RenderableChunkMesh& worldMesh);
    std::map<ChunkID,RenderMesh> chunkMeshes;
    // void setupTestMeshes( int atlasWidth, int atlasHeight);
    bool setupCubeOutline();
    LocInt worldLocToRenderLoc(const LocInt& loc);

    RendererUIData getRendererUIData();
};


#endif //RENDERER_HPP