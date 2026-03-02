#include <render/renderer.hpp>

unsigned int testVAO;
unsigned int testVBO;




Renderer::Renderer(){
}


void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
} 

glm::vec2 jsonGet(const nlohmann::json& data, const std::string& textureName, int atlasWidth, int atlasHeight){
    float xLoc = ((float)data["frames"][textureName]["frame"]["x"])/((float)atlasWidth);
    float yLoc = ((float)data["frames"][textureName]["frame"]["y"])/((float)atlasHeight); 
    return {xLoc, yLoc};
}

LocInt Renderer::worldLocToRenderLoc(const LocInt& loc){
    return {loc.x,loc.y,-loc.z};
}


std::vector<Renderer::chunkVBOElt> Renderer::updateVBOVector(const RenderableChunkMesh& worldMesh){
    std::vector<chunkVBOElt> vertices;
    // the face has nubmered corners starting in the bottom left corner and going counter clockwise.
    // The corners that correspond to the two triangles that make up the face.
    const int cornerOrder[6] = {0,1,3,1,2,3};
    // The uv diffs that correspond the the four corners of the face.
    const std::pair<int,int> uvDiff[] = {{0,1},{1,1},{1,0},{0,0}};
    for( auto face: worldMesh.mesh){
        // std::cout << face.blockType << std::endl;
        // Triangles counter-clockwise
        glm::vec2 uvCoord = jsonGet(jsonAtlasData, BlockRegistry::getTextureName(face.blockType,face.faceType), atlasWidth, atlasHeight);
        for(int i=0;i<6;i++){
            chunkVBOElt vboElt;
            int corner = cornerOrder[i];

           vboElt.pos[0] = face.corners[corner].x;
           vboElt.pos[1] = face.corners[corner].y;
           vboElt.pos[2] = -face.corners[corner].z;
           vboElt.uv[0] = uvCoord.x+(textureMargin)*textureSizeWidth+(1.0f-2.0f*textureMargin)*uvDiff[corner].first*textureSizeWidth;
           vboElt.uv[1] = uvCoord.y+(textureMargin)*textureSizeHeight+(1.0f-2.0f*textureMargin)*uvDiff[corner].second*textureSizeHeight;
           vboElt.tint[0] = face.tint[0];
           vboElt.tint[1] = face.tint[1];
           vboElt.tint[2] = face.tint[2];
           vboElt.tint[3] = 255;

            vertices.push_back(vboElt);
        }
    }
    return vertices;
}

RenderMesh Renderer::createRenderMesh(const RenderableChunkMesh& worldMesh){
    RenderMesh renderMesh;
    glGenVertexArrays(1, &renderMesh.VAO);
    glBindVertexArray(renderMesh.VAO);

    glGenBuffers(1, &renderMesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderMesh.VBO);
    std::vector<chunkVBOElt> vertices = updateVBOVector(worldMesh);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(chunkVBOElt), vertices.data(), GL_STATIC_DRAW);
    renderMesh.nrVertices = worldMesh.mesh.size()*6;
    // Position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(chunkVBOElt), (void*)offsetof(chunkVBOElt, pos));
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(chunkVBOElt), (void*)offsetof(chunkVBOElt, uv));
    glEnableVertexAttribArray(1);
    // tint attribute
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(chunkVBOElt), (void*)offsetof(chunkVBOElt, tint));
    glEnableVertexAttribArray(2);
    return renderMesh;
}

// This is just for testing, delete later!
// void Renderer::setupTestMeshes( int atlasWidth, int atlasHeight){
//     glGenVertexArrays(1,&testVAO);
//     glBindVertexArray(testVAO);

//     glGenBuffers(1,&testVBO);
//     glBindBuffer(GL_ARRAY_BUFFER, testVBO);

//     float atlasRatio = (float) atlasHeight/atlasWidth;
    
//     chunkVBOElt testVertices [] = {
//         {-5,5-10*atlasRatio,-5,0,1,255,255,255,255},
//         {5,5,-5,1,0,255,255,255,255},
//         {-5,5,-5,0,0,255,255,255,255},
//         {5,5,-5,1,0,255,255,255,255},
//         {-5,5-10*atlasRatio,-5,0,1,255,255,255,255},
//         {5,5-10*atlasRatio,-5,1,1,255,255,255,255}
//     };

//     glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
//     glEnableVertexAttribArray(1);

// }


bool Renderer::setupCubeOutline(){

    const float cubeOutlineVertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 0.0f,
    };
    const unsigned int cubeOutlineIndices[] = {
        0,1,
        1,2,
        2,3,
        3,0,
        4,5,
        5,6,
        6,7,
        7,4,
        0,4,
        1,5,
        2,6,
        3,7
    };

    glGenVertexArrays(1, &VAOBlockOutline);
    glBindVertexArray(VAOBlockOutline);

    glGenBuffers(1, &VBOBlockOutline);
    glBindBuffer(GL_ARRAY_BUFFER, VBOBlockOutline);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeOutlineVertices), cubeOutlineVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBOBlockOutline);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBlockOutline);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeOutlineIndices), cubeOutlineIndices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    outLineShaderProgram = Shader("include/shaders/cubeOutlineShader.vs","include/shaders/cubeOutlineShader.fs");
    //  chunkShaderProgram = Shader("../include/shaders/chunkShader.vs","../include/shaders/chunkShader.fs");

    return true;

}

bool Renderer::init(int width, int height){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Dont know what it does
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Dont know what it does
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Dont know what it does.
    window = glfwCreateWindow(width,height,  "Minecraft Clone", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window); // Dont know what it does.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }    

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    
//    stbi_set_flip_vertically_on_load(true);  //Dont know what this does.

// Setting up the texture atlas
    unsigned int textureAtlas;
    glGenTextures(1, &textureAtlas);  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas);  

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int  nrChannels;
    unsigned char *data = stbi_load("assets/blockAtlas.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
    std::cout << "atlasWidth: " << atlasWidth << ", atlasHeight: " << atlasHeight << std::endl;

    // The atlasWidth and atlasHeight give the number of pixels in the atlas.
    // textureSizeWidth gives the portion of the image that belongs to a single texture.
    textureSizeWidth = (float)16/(float)atlasWidth;
    textureSizeHeight = (float)16/(float)atlasHeight;


    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    stbi_image_free(data);

//  Shaders:
     chunkShaderProgram = Shader("include/shaders/chunkShader.vs","include/shaders/chunkShader.fs");
    //  chunkShaderProgram = Shader("../include/shaders/chunkShader.vs","../include/shaders/chunkShader.fs");
    chunkShaderProgram.use();
    chunkShaderProgram.setInt("textureAtlas",0);

    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);

    // The json contains a dictionary from the entity names to the locations on the texture atlas.
    std::ifstream f("assets/blockAtlasJson.json");
    jsonAtlasData = nlohmann::json::parse(f);

    // setupTestMeshes(atlasWidth, atlasHeight);

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


    if(!setupCubeOutline()){
        return false;
    }

    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}




void Renderer::render(World& world, Camera& camera, GameUIData gameData){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    // Custom IMGUI window with game data.
    if(showGameData){
        CameraUIData cameraUIData = camera.getUIData();
        WorldUIData worldUIData = world.getUIData();
        ImGui::Begin("Game Data", &showGameData);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if(gameData.paused)
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
            ImGui::SliderFloat("Texture margin",&textureMargin,0.0f,0.02f);
            ImGui::SliderFloat("Local Outline Offset",&localOutlineOffset,0.0f,0.02f);
            ImGui::SliderFloat("Local Outline Width",&localOutlineWidth,0.0f,10.0f);

        }
        // if (ImGui::Button("Close Me"))
        //     show_another_window = false;
        ImGui::End();
    }


    //Chunk rendering starts here:

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate the view and projection matrices
    glm::mat4 view;
    view = camera.getViewMatrix();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.getFov()),  (float)width / (float)height, 0.1f, 300.0f);

    // Set the view and projection matrices to the right values in the chunk shader program
    chunkShaderProgram.use();

    unsigned int viewLoc = glGetUniformLocation(chunkShaderProgram.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    unsigned int projectionLoc = glGetUniformLocation(chunkShaderProgram.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Loop through the renderable chunk meshes.
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue = world.toRenderableChunkQueue();
    while(!chunkQueue.empty()){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunkQueue.front();
        chunkQueue.pop();
        ChunkID chunkID = chunkPtr->chunkId;
        if(chunkMeshes.count(chunkID)==0){ // If we havent rendered the chunk before, we set up the VAO and VBO.
            chunkMeshes[chunkID] = createRenderMesh(*chunkPtr);
            glBindVertexArray(chunkMeshes[chunkID].VAO);
        } else if(chunkPtr->updated){ // The chunk was updated so we recalculate the VBO.
            std::vector<chunkVBOElt> vertices = updateVBOVector(*chunkPtr);
            chunkMeshes[chunkID].nrVertices = 6*chunkPtr->mesh.size();
            glBindVertexArray(chunkMeshes[chunkID].VAO);
            glBindBuffer(GL_ARRAY_BUFFER,chunkMeshes[chunkID].VBO);
            // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(chunkVBOElt), vertices.data());
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(chunkVBOElt), vertices.data(), GL_STATIC_DRAW);
            chunkPtr->updated = false;
        } else {
            glBindVertexArray(chunkMeshes[chunkID].VAO);
        }

        glDrawArrays(GL_TRIANGLES, 0, chunkMeshes[chunkID].nrVertices);
    }


    // Highlight the selected cube.
    if( world.hasBlockTargeted()){
        // glDisable(GL_DEPTH_TEST);
        LocInt targetedBlock = world.getTargetedBlock();
        glBindVertexArray(VAOBlockOutline);
        outLineShaderProgram.use();

        outLineShaderProgram.setMat4("view",view);
        outLineShaderProgram.setMat4("projection",projection);

        outLineShaderProgram.setLocInt("offset",worldLocToRenderLoc(targetedBlock));
        glLineWidth(localOutlineWidth);
        glm::vec3 localOffsets[] = {    glm::vec3(localOutlineOffset,localOutlineOffset,0),
                                        glm::vec3(-localOutlineOffset,localOutlineOffset,0),
                                        glm::vec3(-localOutlineOffset,-localOutlineOffset,0),
                                        glm::vec3(localOutlineOffset,-localOutlineOffset,0),
                                        glm::vec3(localOutlineOffset,0,localOutlineOffset),
                                        glm::vec3(-localOutlineOffset,0,localOutlineOffset),
                                        glm::vec3(-localOutlineOffset,0,-localOutlineOffset),
                                        glm::vec3(localOutlineOffset,0,-localOutlineOffset),
                                        glm::vec3(0,localOutlineOffset,localOutlineOffset),
                                        glm::vec3(0,-localOutlineOffset,localOutlineOffset),
                                        glm::vec3(0,-localOutlineOffset,-localOutlineOffset),
                                        glm::vec3(0,localOutlineOffset,-localOutlineOffset)};
        for(int i=0; i<12; i++){
            outLineShaderProgram.setVec3("localOffset",localOffsets[i]);
            glDrawElements(GL_LINES,24, GL_UNSIGNED_INT, 0);
        }
        glLineWidth(1.0f);

        // glEnable(GL_DEPTH_TEST);
    }







    // ImGui rendering:
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    glfwSwapBuffers(window);
}


void Renderer::shutDown(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}