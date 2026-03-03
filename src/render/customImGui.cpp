#include <render/customImGui.hpp>


void CustomImGui::setup(GLFWwindow* window){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO&  io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}


void CustomImGui::renderStart(CameraUIData cameraUIData, WorldUIData worldUIData, GameUIData gameUIData, RendererUIData rendererUIData){
    static bool showGameData = true;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    // Custom IMGUI window with game data.
    if(showGameData){
        ImGui::Begin("Game Data", &showGameData);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if(gameUIData.paused)
            ImGui::Text("Game is paused!");
        else
            ImGui::Text("Game is unpaused!");
        if (ImGui::CollapsingHeader("Camera",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::SliderFloat("Camera fov (degrees)",cameraUIData.fovP,1.0f,179.0f);
        }
        if (ImGui::CollapsingHeader("Player",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Text("Player location(xyz): %f,%f,%f",worldUIData.playerData.pos.x,worldUIData.playerData.pos.y,worldUIData.playerData.pos.z);
            ImGui::Text("Looking direction(xyz): %f,%f,%f",worldUIData.playerData.forwardDirection.x,worldUIData.playerData.forwardDirection.y,worldUIData.playerData.forwardDirection.z);
            ImGui::SliderFloat("Movement Speed",worldUIData.playerData.playerSpeedP,1.0f,100.0f);
            ImGui::SliderFloat("Mouse Sensitivity",worldUIData.playerData.rotationSensitivityP,0.01f,0.5f);
        }
        if (ImGui::CollapsingHeader("World",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::SliderInt("Block Delay (ticks)",worldUIData.ticksBetweenBlockManipulationP,1,20);
            ImGui::SliderFloat("Tick Length (seconds)",worldUIData.tickTimeLengthP,0.01f,1.0f);
            if(worldUIData.blockTargeted){
                ImGui::Text("Block targeted(xyz): %d,%d,%d",worldUIData.targetedBlock.x,worldUIData.targetedBlock.y,worldUIData.targetedBlock.z);
            } else {
                ImGui::Text("No block targeted.");
            }
        }
        if (ImGui::CollapsingHeader("Renderer",ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::SliderFloat("Texture margin",rendererUIData.textureMarginP,0.0f,0.02f);
            ImGui::SliderFloat("Local Outline Offset",rendererUIData.localOutlineOffsetP,0.0f,0.02f);
            ImGui::SliderFloat("Local Outline Width",rendererUIData.localOutlineWidthP,0.0f,10.0f);
            ImGui::SliderFloat("Hotbar Width Portion",rendererUIData.hotbarWidthPortionP,0.0f,1.0f);
            ImGui::SliderFloat("Crosshair Length",rendererUIData.crossHairLengthP,0.0f,0.2f);
            ImGui::SliderFloat("Crosshair Width",rendererUIData.crossHairWidthP,0.0f,0.10f);
        }
        // if (ImGui::Button("Close Me"))
        //     show_another_window = false;
        ImGui::End();
    }
}
void CustomImGui::renderEnd(){
    // ImGui rendering:
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CustomImGui::shutdown(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}