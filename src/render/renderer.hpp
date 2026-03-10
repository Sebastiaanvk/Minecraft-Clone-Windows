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
#include <algorithm>
#include <chrono>
#include <util/macros.hpp>



class RenderMesh{
    public:
    unsigned int VAO;
    unsigned int VBO;
    GLsizei nrVertices;
};
// class ChunkMeshes{
//     RenderMesh solidMesh;
//     RenderMesh cutoutMesh;
// };

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

    struct CutoutVBOElt{
        glm::vec3 pos;
        glm::vec2 uv;
        // uint8_t tint[4];
    };

    bool showGameData = true;
    float projectionDistance = 500.0f;
    float textureMargin = 0.005f;
    int maxNewMeshesPerFrame = 3;
    bool vSync =false;

    GLFWwindow* window;

    LocInt worldLocToRenderLoc(const LocInt& loc);

    Shader chunkShaderProgram;
    unsigned int viewLocChunks;
    unsigned int projectionLocChunks;
    std::map<ChunkID,RenderMesh> solidMeshes;
    std::map<ChunkID,RenderMesh> cutoutMeshes;
    std::vector<chunkVBOElt> updateVBOVector(const RenderableChunkMesh& worldMesh);
    RenderMesh createRenderMesh(const RenderableChunkMesh& worldMesh);
    void renderChunks(World& world, glm::mat4& view, glm::mat4& projection);

    float localOutlineOffset = 0.002f;
    float localOutlineWidth = 7.0f;
    unsigned int VAOBlockOutline;
    Shader outLineShaderProgram;
    bool setupCubeOutline();
    void renderHighlightedCube(const World& world, const glm::mat4& view, const glm::mat4& projection);

    Shader rectangleShaderProgram;
    unsigned int VAORectangle;
    bool setupRectangleRenderer();

    Shader uiTextureShaderProgram;
    unsigned int VAO2dTexture;
    bool setup2dRenderer();

    float hotbarWidthPortion = 0.7f;
    unsigned int hotbarTextureAtlas;
    bool setupHotbarTexture();
    void renderHotbar(const World& world);
     
    float crossHairLength = 0.05f; //These ratios are with respect to the height of the window.
    float crossHairWidth = 0.005f;
    void renderCrosshair();

    RendererUIData getRendererUIData();

    // void renderHotbarTest();
    // void setupTestMeshes( int atlasWidth, int atlasHeight);

};


#endif //RENDERER_HPP