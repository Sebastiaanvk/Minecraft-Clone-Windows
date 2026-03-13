#include <render/renderer.hpp>

Renderer::Renderer():
    chunkRenderer(*this),uiRenderer(*this){

}

void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
} 

void Renderer::render(World& world, Camera& camera, GameUIData gameData){
    getRendererUIData();
    CustomImGui::renderStart(camera.getUIData(), world.getUIData(), gameData, getRendererUIData());

    //Chunk rendering starts here:
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate the view and projection matrices
    glm::mat4 view;
    view = camera.getViewMatrix();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.getFov()),  (float)width / (float)height, 0.1f, renderSettings.projectionDistance);

    // START_TIMING(renderChunksN)
    chunkRenderer.renderChunks(world, view, projection);
    // END_TIMING(renderChunksN)

    // Highlight the selected cube.
    if( world.hasBlockTargeted() && !world.playerIsUnderwater()){
        uiRenderer.renderHighlightedCube(world, view, projection);
    }
    if(world.playerIsUnderwater()){
    }

    // Render UI
    uiRenderer.renderHotbar(world);
    uiRenderer.renderCrosshair();

    CustomImGui::renderEnd();

    glfwSwapBuffers(window);
}



bool Renderer::init(int width, int height){
    // Generic GLFW window stuff.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    window = glfwCreateWindow(width,height,  "Minecraft Clone", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window); 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }    

    glViewport(0, 0, width, height);
    // Changes the size of the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  



    
//    stbi_set_flip_vertically_on_load(true);  //This changes the texture to have 0,0 as top left instead of the default bottom left.
// setup the large texture atlas for all block textures.
    if(!TextureAtlas::setup())
        return false;

    if(!chunkRenderer.init())
        return false;

    // Both of these are the defaults! For some renders we disable, but make sure to enable again.
    glEnable(GL_DEPTH_TEST);  
    // Only render triangles going counter clockwise.
    glEnable(GL_CULL_FACE);

    CustomImGui::setup(window);

    if (!uiRenderer.init())
        return false;


    if(vSync)
        glfwSwapInterval(1); // Enables VSync
    else
        glfwSwapInterval(0); // Enables VSync

    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}

void Renderer::shutDown(){
    CustomImGui::shutdown();
}


RendererUIData Renderer::getRendererUIData(){
    RendererUIData rendererUIData;
    rendererUIData.maxNewMeshesPerFrameP = &renderSettings.maxNewMeshesPerFrame;
    rendererUIData.projectionDistanceP = &renderSettings.projectionDistance;
    rendererUIData.textureMarginP = &renderSettings.textureMargin;
    rendererUIData.localOutlineOffsetP = &renderSettings.localOutlineOffset;
    rendererUIData.localOutlineWidthP = &renderSettings.localOutlineWidth;
    rendererUIData.hotbarWidthPortionP = &renderSettings.hotbarWidthPortion;
    rendererUIData.crossHairLengthP = &renderSettings.crossHairLength;
    rendererUIData.crossHairWidthP = &renderSettings.crossHairWidth;

    return rendererUIData;
}

