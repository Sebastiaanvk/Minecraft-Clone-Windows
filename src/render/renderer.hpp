#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <world/world.hpp>
#include <camera.hpp>
#include <render/uiData.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/customImGui.hpp>
#include <chrono>
#include <render/chunkRenderer.hpp>
#include <render/uiRenderer.hpp>
// #include <memory>
// #include <render/renderable.hpp>
// #include <util/loc.h>
#include <util/macros.hpp>



class Renderer {
public:
    Renderer();

    bool init(int width, int height);

    GLFWwindow* getWindow();

    void render(World& world, Camera& camera, GameUIData gameData);
    void shutDown();

    float getAspectRatio();

    const int blockTextureSlotOffset = 0;
    const int hotbarTextureSlotOffset = 1;

    struct RenderSettings{
        float projectionFarDistance = 1000.0f;
        float projectionNearDistance = 0.1f;
        float textureMargin = 0.005f;
        int maxNewMeshesPerFrame = 10;
        float localOutlineOffset = 0.002f;
        float localOutlineWidth = 7.0f;
        float hotbarWidthPortion = 0.7f;
        float crossHairLength = 0.05f; //These ratios are with respect to the height of the window.
        float crossHairWidth = 0.005f;

        int maxTerrainRenders = 2000;
        int maxWaterRenders = 2000;
        int maxVegetationRenders = 1600; 

        bool ambientOcclusion = true;
        bool gammaCorrection = true;
    };
    RenderSettings renderSettings;

private:
    
    bool showGameData = true;
    bool vSync =false;

    GLFWwindow* window;

    ChunkRenderer chunkRenderer;
    UIRenderer uiRenderer;

    RendererUIData getRendererUIData();
};


#endif //RENDERER_HPP