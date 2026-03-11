#ifndef CUSTOMIMGUI_HPP
#define CUSTOMIMGUI_HPP


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <render/uiData.hpp>
#include <util/loc.h>

namespace  CustomImGui{
    void setup(GLFWwindow* window);
    void renderStart(CameraUIData cameraUIData, WorldUIData worldUIData, GameUIData gameUIData, RendererUIData rendererUIData);
    void renderEnd();
    void shutdown();
}

#endif //CUSTOMIMGUI_HPP