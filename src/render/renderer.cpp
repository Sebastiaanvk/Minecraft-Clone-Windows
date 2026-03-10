#include <render/renderer.hpp>

Renderer::Renderer(){
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
    projection = glm::perspective(glm::radians(camera.getFov()),  (float)width / (float)height, 0.1f, projectionDistance);

    START_TIMING(renderChunksN)
    renderChunks(world, view, projection);
    END_TIMING(renderChunksN)

    // Highlight the selected cube.
    if( world.hasBlockTargeted()){
        renderHighlightedCube(world, view, projection);
    }

    // Render UI
    renderHotbar(world);
    renderCrosshair();

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
    if(!TextureAtlas::setup()){
        return false;
    }

//  Shaders:
    chunkShaderProgram = Shader("src/shaders/chunkShader.vs","src/shaders/chunkShader.fs");
    chunkShaderProgram.use();
    chunkShaderProgram.setInt("textureAtlas",0);
    viewLocChunks = glGetUniformLocation(chunkShaderProgram.ID, "view");
    projectionLocChunks = glGetUniformLocation(chunkShaderProgram.ID, "projection");



    glEnable(GL_DEPTH_TEST);  
    // Only render triangles going counter clockwise.
    glEnable(GL_CULL_FACE);


    CustomImGui::setup(window);

    if(!setupCubeOutline()){
        return false;
    }

    if(!setup2dRenderer()){
        return false;
    }

    if(!setupHotbarTexture()){
        return false;
    }
    
    if(!setupRectangleRenderer()){
        return false;
    }



    if(vSync)
        glfwSwapInterval(1); // Enables VSync
    else
        glfwSwapInterval(0); // Enables VSync

    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}


LocInt Renderer::worldLocToRenderLoc(const LocInt& loc){
    return {loc.x,loc.y,-loc.z};
}

void Renderer::shutDown(){
    CustomImGui::shutdown();
}

void Renderer::renderChunks(World& world, glm::mat4& view, glm::mat4& projection){
    // Set the view and projection matrices to the right values in the chunk shader program
    chunkShaderProgram.use();
    TextureAtlas::bind();
    glUniformMatrix4fv(viewLocChunks, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocChunks, 1, GL_FALSE, glm::value_ptr(projection));


    std::vector<std::shared_ptr<RenderableChunkMesh>> chunksToUploadGPU;
    std::vector<ChunkID> chunksToRender;

    // Loop through the renderable chunk meshes.

    START_TIMING(renderableChunkQueue)
    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue = world.toRenderableChunkQueue();
    END_TIMING(renderableChunkQueue)

    START_TIMING(drawLoadedChunks)
    while(!chunkQueue.empty()){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunkQueue.front();
        chunkQueue.pop();
        ChunkID chunkID = chunkPtr->chunkId;
        if(solidMeshes.count(chunkID)!=0 && !chunkPtr->updated){
            glBindVertexArray(solidMeshes[chunkID].VAO);
            glDrawArrays(GL_TRIANGLES, 0, solidMeshes[chunkID].nrVertices);
            chunksToRender.push_back(chunkID);
        } else {
            chunksToUploadGPU.push_back(chunkPtr);
        }
    }
    END_TIMING(drawLoadedChunks)
    LocInt playerLoc = world.player.getBlockLoc();

    if(chunksToUploadGPU.size()>maxNewMeshesPerFrame){
        std::nth_element(chunksToUploadGPU.begin(),chunksToUploadGPU.begin()+maxNewMeshesPerFrame-1,chunksToUploadGPU.end(),
            [playerLoc](const std::shared_ptr<RenderableChunkMesh>& m1, const std::shared_ptr<RenderableChunkMesh>& m2){
                int dx1 = (m1->chunkId.x-playerLoc.x);
                int dz1 = (m1->chunkId.z-playerLoc.z);
                int dx2 = (m2->chunkId.x-playerLoc.x);
                int dz2 = (m2->chunkId.z-playerLoc.z);
                return dx1*dx1+dz1*dz1<dx2*dx2+dz2*dz2;
            }
        );
    }
    START_TIMING(drawUnloadedChunks)
    for(int i=0; i<maxNewMeshesPerFrame && i<chunksToUploadGPU.size(); i++){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunksToUploadGPU[i];
        ChunkID chunkID = chunkPtr->chunkId;
        if( solidMeshes.count(chunkID) ){
            std::vector<chunkVBOElt> vertices = updateVBOVector(*chunkPtr);
            solidMeshes[chunkID].nrVertices = 6*chunkPtr->solidMesh.size();
            glBindVertexArray(solidMeshes[chunkID].VAO);
            glBindBuffer(GL_ARRAY_BUFFER,solidMeshes[chunkID].VBO);
            // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(chunkVBOElt), vertices.data());
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(chunkVBOElt), vertices.data(), GL_STATIC_DRAW);
            chunkPtr->updated = false;
        } else {
            solidMeshes[chunkID] = createRenderMesh(*chunkPtr);
            glBindVertexArray(solidMeshes[chunkID].VAO);
        }
        chunksToRender.push_back(chunkID);
        glDrawArrays(GL_TRIANGLES, 0, solidMeshes[chunkID].nrVertices);
    }
    END_TIMING(drawUnloadedChunks)

        // if(chunkMeshes.count(chunkID)==0){ // If we havent rendered the chunk before, we set up the VAO and VBO.
        //     chunkMeshes[chunkID] = createRenderMesh(*chunkPtr);
        //     glBindVertexArray(chunkMeshes[chunkID].VAO);
        // } else if(chunkPtr->updated){ // The chunk was updated so we recalculate the VBO.
        //     std::vector<chunkVBOElt> vertices = updateVBOVector(*chunkPtr);
        //     chunkMeshes[chunkID].nrVertices = 6*chunkPtr->mesh.size();
        //     glBindVertexArray(chunkMeshes[chunkID].VAO);
        //     glBindBuffer(GL_ARRAY_BUFFER,chunkMeshes[chunkID].VBO);
        //     // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(chunkVBOElt), vertices.data());
        //     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(chunkVBOElt), vertices.data(), GL_STATIC_DRAW);
        //     chunkPtr->updated = false;
        // } else {
        //     glBindVertexArray(chunkMeshes[chunkID].VAO);
        // }

        // glDrawArrays(GL_TRIANGLES, 0, chunkMeshes[chunkID].nrVertices);
}

std::vector<Renderer::chunkVBOElt> Renderer::updateVBOVector(const RenderableChunkMesh& worldMesh){
    std::vector<chunkVBOElt> vertices;
    // The face has numbered corners starting in the bottom left corner and going counter clockwise.
    // The corners that correspond to the two triangles that make up the face.
    const int cornerOrder[6] = {0,1,3,1,2,3};
    // The uv diffs that correspond to the four corners of the face.
    const std::pair<int,int> uvDiff[] = {{0,1},{1,1},{1,0},{0,0}};
    for( auto face: worldMesh.solidMesh){
        // Triangles counter-clockwise
        glm::vec2 uvCoord = TextureAtlas::getUVCoord(face.blockType,face.faceType);
        for(int i=0;i<6;i++){
            chunkVBOElt vboElt;
            int corner = cornerOrder[i];
            
            vboElt.pos[0] = face.corners[corner].x;
            vboElt.pos[1] = face.corners[corner].y;
            vboElt.pos[2] = -face.corners[corner].z;
            vboElt.uv[0] = uvCoord.x+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].first)*TextureAtlas::getTextureSizeWidth();
            vboElt.uv[1] = uvCoord.y+(textureMargin+(1.0f-2.0f*textureMargin)*uvDiff[corner].second)*TextureAtlas::getTextureSizeHeight();
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
    renderMesh.nrVertices = worldMesh.solidMesh.size()*6;
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

RendererUIData Renderer::getRendererUIData(){
    RendererUIData rendererUIData;
    rendererUIData.maxNewMeshesPerFrameP = &maxNewMeshesPerFrame;
    rendererUIData.projectionDistanceP = &projectionDistance;
    rendererUIData.textureMarginP = &textureMargin;
    rendererUIData.localOutlineOffsetP = &localOutlineOffset;
    rendererUIData.localOutlineWidthP = &localOutlineWidth;
    rendererUIData.hotbarWidthPortionP = &hotbarWidthPortion;
    rendererUIData.crossHairLengthP = &crossHairLength;
    rendererUIData.crossHairWidthP = &crossHairWidth;

    return rendererUIData;
}

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

    unsigned int VBOBlockOutline; 
    glGenBuffers(1, &VBOBlockOutline);
    glBindBuffer(GL_ARRAY_BUFFER, VBOBlockOutline);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeOutlineVertices), cubeOutlineVertices, GL_STATIC_DRAW);
    
    unsigned int EBOBlockOutline;
    glGenBuffers(1, &EBOBlockOutline);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBlockOutline);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeOutlineIndices), cubeOutlineIndices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    outLineShaderProgram = Shader("src/shaders/cubeOutlineShader.vs","src/shaders/cubeOutlineShader.fs");
    return true;
}

void Renderer::renderHighlightedCube(const World& world, const glm::mat4& view, const glm::mat4& projection){
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
}


bool Renderer::setup2dRenderer(){
    // negative y means going down in opengl 2d space, but going up in the texture atlas
    // These are Normalized Device Coordinates  in [-1,1]x[-1,1]
    // x,y,u,v (Note that we pick 0,0 as top left for both)
    const float vertices[] = {
        0.0f,-1.0f,0.0f,1.0f, //lower left corner
        1.0f,-1.0f,1.0f,1.0f,
        1.0f,0.0f,1.0f,0.0f,  
        0.0f,0.0f,0.0f,0.0f // Upper left corner
    };
    const unsigned int indices[] = {0,1,3,1,2,3}; // Two counter clockwise triangles.

    unsigned int VBO2dTexture; 
    unsigned int EBO2dTexture;
    glGenVertexArrays(1, &VAO2dTexture);
    glBindVertexArray(VAO2dTexture);
    glGenBuffers(1, &VBO2dTexture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2dTexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO2dTexture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2dTexture);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    uiTextureShaderProgram = Shader("src/shaders/uiShader.vs","src/shaders/uiShader.fs");
    uiTextureShaderProgram.setInt("textureAtlas",0);

    return true;
}
bool Renderer::setupHotbarTexture(){

    // Setting up the texture atlas
    glGenTextures(1, &hotbarTextureAtlas);  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hotbarTextureAtlas);  

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int  nrChannels;
    int atlasWidth,atlasHeight;
    unsigned char *data = stbi_load("assets/minecraftHotbarTexture.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
    // std::cout << "atlasWidth: " << atlasWidth << ", atlasHeight: " << atlasHeight << std::endl;

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Not sure if necessary.
    } else {
        std::cout << "Failed to load hotbar texture." << std::endl;
        return false;
    }
    stbi_image_free(data);
    return true;
}


void Renderer::renderHotbar(const World& world){
    // In the real game, the htobar doesnt scale directly with the window size
    // Instead it jumps in size at certain window sizes. Probably so the pixels align with the texture nicely.
    glBindVertexArray(VAO2dTexture);
    glBindTexture(GL_TEXTURE_2D,hotbarTextureAtlas);
    glDisable(GL_DEPTH_TEST);
    TextureLoc hotbarTexture = {0,0,182,22}; //The full png is 256x64.
    int textureWidth = 256;
    int textureHeight = 64;
    glm::vec2 hotbarTextureLoc = glm::vec2(0,0);
    glm::vec2 hotbarTextureSize = glm::vec2((float)hotbarTexture.width/textureWidth,(float)hotbarTexture.height/textureHeight);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float hotbarScreenHeight = 2*hotbarWidthPortion*((float)hotbarTexture.height/hotbarTexture.width)*((float)width/height);
    glm::vec2 size = glm::vec2(2*hotbarWidthPortion,hotbarScreenHeight);
    glm::vec2 loc = glm::vec2(-hotbarWidthPortion,-1.0f+hotbarScreenHeight);
    
    uiTextureShaderProgram.use();
    uiTextureShaderProgram.setVec2("loc",loc);
    uiTextureShaderProgram.setVec2("size",size);
    uiTextureShaderProgram.setVec2("textureLoc",hotbarTextureLoc);
    uiTextureShaderProgram.setVec2("textureSize",hotbarTextureSize);
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

// Alright, this could have been done in a way nicer way, I'm sure
// Just want something that works right now.
    glm::vec2 selectionBoxTextureLoc = glm::vec2(0,(float)22/64);
    glm::vec2 selectionBoxTextureSize = glm::vec2((float)24/256,(float)24/64);

    glm::vec2 selectionBoxSize = glm::vec2((float)24/182*size.x,(float)24/22*size.y);
    float selectionBoxXLoc = (float)(182+2)/182*loc.x + world.getHotbarSelection()*size.x*20/182;
    glm::vec2 selectionBoxLoc = glm::vec2(selectionBoxXLoc,-1.0f+((float)23/22)*hotbarScreenHeight);


    uiTextureShaderProgram.setVec2("loc",selectionBoxLoc);
    uiTextureShaderProgram.setVec2("size",selectionBoxSize);
    uiTextureShaderProgram.setVec2("textureLoc",selectionBoxTextureLoc);
    uiTextureShaderProgram.setVec2("textureSize",selectionBoxTextureSize);
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    TextureAtlas::bind();
    RenderableInventory renderableInventory = world.getRenderableInventory();
    float pixelsTextures = 10.0f; // Just for now. These are the number of pixels of the original hotbar texture. Not actual pixels on the screen.
    float itemCornerOffset = (20.0f-pixelsTextures)/2; // The squares in the hotbar texture have size 20x20.

    float cornerX = (float)(91-1-itemCornerOffset)/91*loc.x; // The hotbar Texture is 182 pixels wide. There is one extra column on the side.
    float cornerY = -1.0f+(float)(22-1-itemCornerOffset)/22*hotbarScreenHeight;
    glm::vec2 itemSize = glm::vec2(pixelsTextures/182*size.x,pixelsTextures/22*size.y);
    uiTextureShaderProgram.setVec2("size",itemSize);
    for(int i=0; i<9; i++){
        if(renderableInventory.slotOccupied[i]){
            uiTextureShaderProgram.setVec2("loc",glm::vec2(cornerX+(float)i*20/182*size.x,cornerY));
            uiTextureShaderProgram.setVec2("textureLoc",TextureAtlas::getUVCoord(renderableInventory.slotContents[i],FaceType::Side));
            uiTextureShaderProgram.setVec2("textureSize",TextureAtlas::getTextureSize());
            glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
        }
    }


    glEnable(GL_DEPTH_TEST);
}
// void Renderer::renderHotbarTest(){
//     glBindVertexArray(VAO2dTexture);
//     glBindTexture(GL_TEXTURE_2D,hotbarTextureAtlas);
//     glDisable(GL_DEPTH_TEST);
//     // TextureLoc hotbarTexture = {0,0,182,22}; //The full png is 256x64.
//     // int textureWidth = 256;
//     // int textureHeight = 64;
//     glm::vec2 hotbarTextureLoc = glm::vec2(0.0f,0.0f);
//     glm::vec2 hotbarTextureSize = glm::vec2(1.0f,1.0f);
    
//     // int width, height;
//     // glfwGetWindowSize(window, &width, &height);
//     // float hotbarScreenHeight = 2*hotbarWidthPortion*((float)hotbarTexture.height/hotbarTexture.width)*((float)width/height);
//     glm::vec2 size = glm::vec2(1.0f,1.0f);
//     glm::vec2 loc = glm::vec2(-0.5f,0.5f);
    
//     uiTextureShaderProgram.use();
//     uiTextureShaderProgram.setVec2("loc",loc);
//     uiTextureShaderProgram.setVec2("size",size);
//     uiTextureShaderProgram.setVec2("textureLoc",hotbarTextureLoc);
//     uiTextureShaderProgram.setVec2("textureSize",hotbarTextureSize);
//     glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

//     glEnable(GL_DEPTH_TEST);
// }


bool Renderer::setupRectangleRenderer(){

    // negative y means going down in opengl 2d space
    // These are Normalized Device Coordinates  in [-1,1]x[-1,1]
    // x,y 
    const float vertices[] = {
        0.0f,-1.0f, //lower left corner
        1.0f,-1.0f,
        1.0f,0.0f,  
        0.0f,0.0f // Upper left corner
    };
    const unsigned int indices[] = {0,1,3,1,2,3}; // Two counter clockwise triangles.

    unsigned int VBORectangle; 
    unsigned int EBORectangle;
    glGenVertexArrays(1, &VAORectangle);
    glBindVertexArray(VAORectangle);
    glGenBuffers(1, &VBORectangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBORectangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBORectangle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORectangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // Maybe change so that we get the pixel position as input
    // and we multiply by an orthographic matrix?
    rectangleShaderProgram = Shader("src/shaders/rectangleShader.vs","src/shaders/rectangleShader.fs");
    return true;
}

void Renderer::renderCrosshair(){
    glBindVertexArray(VAORectangle);
    glDisable(GL_DEPTH_TEST);
    rectangleShaderProgram.use();
    rectangleShaderProgram.setVec4("color",glm::vec4(0.3f,0.3f,0.3f,1.0f));
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float widthHeightRatio = (float)width/height; //Generally >1.
    rectangleShaderProgram.setVec2("loc",glm::vec2(-crossHairWidth/widthHeightRatio,crossHairLength));
    rectangleShaderProgram.setVec2("size",glm::vec2(2*crossHairWidth/widthHeightRatio,2*crossHairLength));
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    rectangleShaderProgram.setVec2("loc",glm::vec2(-crossHairLength/widthHeightRatio,crossHairWidth));
    rectangleShaderProgram.setVec2("size",glm::vec2(2*crossHairLength/widthHeightRatio,2*crossHairWidth));
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}