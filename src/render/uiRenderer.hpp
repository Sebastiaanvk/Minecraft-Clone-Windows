#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

// #include <render/renderer.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders/shader.h>
#include <render/textures.hpp>
#include <world/world.hpp>
#include <util/macros.hpp>
#include <util/loc.h>

class Renderer;

class UIRenderer{
public:
    UIRenderer(Renderer& renderer);
    bool init();
    void renderHighlightedCube(const World& world, const glm::mat4& view, const glm::mat4& projection);
    void renderUnderwater();
    void renderHotbar(const World& world);
    void renderCrosshair();

private:
    Renderer& renderer;

    const float& localOutlineOffset;
    const float& localOutlineWidth;
    unsigned int VAOBlockOutline;
    Shader outLineShaderProgram;
    bool setupCubeOutline();

    // unsigned int VAOUnderwater;

    Shader rectangleShaderProgram;
    unsigned int VAORectangle;
    bool setupRectangleRenderer();

    Shader uiTextureShaderProgram;
    unsigned int VAO2dTexture;
    bool setup2dRenderer();

    const float& hotbarWidthPortion ;
    unsigned int hotbarTextureAtlas;
    bool setupHotbarTexture();
     
    const float& crossHairLength; //These ratios are with respect to the height of the window.
    const float& crossHairWidth ;
};




#endif //UI_RENDERER_HPP