#include <render/uiRenderer.hpp>
#include <render/renderer.hpp>


UIRenderer::UIRenderer(Renderer& renderer) :
    renderer(renderer), localOutlineOffset(renderer.renderSettings.localOutlineOffset), localOutlineWidth(renderer.renderSettings.localOutlineWidth),
    hotbarWidthPortion(renderer.renderSettings.hotbarWidthPortion),crossHairLength(renderer.renderSettings.crossHairLength),
    crossHairWidth(renderer.renderSettings.crossHairWidth){
}

bool UIRenderer::init(){

    if(!setupCubeOutline()){
        return false;
    }

    if(!setupHotbarTexture()){
        return false;
    }

    if(!setup2dRendererAtlas()){
        return false;
    }
    if(!setup2dRendererArray()){
        return false;
    }

    
    if(!setupRectangleRenderer()){
        return false;
    }
    return true;
}

bool UIRenderer::setupCubeOutline(){

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

    outLineShaderProgram = Shader("src/shaders/uiShaders/cubeOutlineShader.vs","src/shaders/uiShaders/cubeOutlineShader.fs");
    return true;
}

void UIRenderer::renderHighlightedCube(const World& world, const glm::mat4& view, const glm::mat4& projection){
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


bool UIRenderer::setup2dRendererAtlas(){
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
    glGenVertexArrays(1, &VAO2dTextureAtlas);
    glBindVertexArray(VAO2dTextureAtlas);
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

    uiTextureShaderProgramAtlas = Shader("src/shaders/uiShaders/uiShaderAtlas.vs","src/shaders/uiShaders/uiShaderAtlas.fs");
    uiTextureShaderProgramAtlas.use();
    uiTextureShaderProgramAtlas.setInt("textureAtlas",renderer.hotbarTextureSlotOffset);
    uiTextureShaderProgramAtlas.setVec4("tint",glm::vec4(1.0f,1.0f,1.0f,1.0f)); // We leave the tint at (1.0,1.0,1.0,1.0) as default.

    return true;
}

bool UIRenderer::setup2dRendererArray(){
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
    glGenVertexArrays(1, &VAO2dTextureArray);
    glBindVertexArray(VAO2dTextureArray);
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

    uiTextureShaderProgramArray = Shader("src/shaders/uiShaders/uiShaderArray.vs","src/shaders/uiShaders/uiShaderArray.fs");
    uiTextureShaderProgramArray.use();
    uiTextureShaderProgramArray.setInt("textureArray",renderer.blockTextureSlotOffset);
    uiTextureShaderProgramArray.setVec4("tint",glm::vec4(1.0f,1.0f,1.0f,1.0f)); // We leave the tint at (1.0,1.0,1.0,1.0) as default.

    return true;
}


bool UIRenderer::setupHotbarTexture(){

    // Setting up the texture atlas
    glActiveTexture(GL_TEXTURE0+renderer.hotbarTextureSlotOffset);
    glGenTextures(1, &hotbarTextureAtlas);  
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Not sure if necessary.
    } else {
        std::cout << "Failed to load hotbar texture." << std::endl;
        return false;
    }
    stbi_image_free(data);
    // glActiveTexture(GL_TEXTURE0);
    return true;
}

void UIRenderer::renderHotbar(const World& world){
    // In the real game, the htobar doesnt scale directly with the window size
    // Instead it jumps in size at certain window sizes. Probably so the pixels align with the texture nicely.
    glBindVertexArray(VAO2dTextureAtlas);
    // glActiveTexture(GL_TEXTURE0+renderer.hotbarTextureSlotOffset);
    // glBindTexture(GL_TEXTURE_2D, hotbarTextureAtlas);  
    glDisable(GL_DEPTH_TEST);
    TextureLoc hotbarTexture = {0,0,182,22}; //The full png is 256x64.
    int textureWidth = 256;
    int textureHeight = 64;
    glm::vec2 hotbarTextureLoc = glm::vec2(0,0);
    glm::vec2 hotbarTextureSize = glm::vec2((float)hotbarTexture.width/textureWidth,(float)hotbarTexture.height/textureHeight);
    
    int width, height;
    glfwGetWindowSize(renderer.getWindow(), &width, &height);
    float hotbarScreenHeight = 2*hotbarWidthPortion*((float)hotbarTexture.height/hotbarTexture.width)*((float)width/height);
    glm::vec2 size = glm::vec2(2*hotbarWidthPortion,hotbarScreenHeight);
    glm::vec2 loc = glm::vec2(-hotbarWidthPortion,-1.0f+hotbarScreenHeight);
    
    uiTextureShaderProgramAtlas.use();
    // uiTextureShaderProgramAtlas.setInt("textureAtlas",renderer.hotbarTextureSlotOffset);
    uiTextureShaderProgramAtlas.setVec2("loc",loc);
    uiTextureShaderProgramAtlas.setVec2("size",size);
    uiTextureShaderProgramAtlas.setVec2("textureLoc",hotbarTextureLoc);
    uiTextureShaderProgramAtlas.setVec2("textureSize",hotbarTextureSize);
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

// Alright, this could have been done in a way nicer way, I'm sure
// Just want something that works right now.
    glm::vec2 selectionBoxTextureLoc = glm::vec2(0,(float)22/64);
    glm::vec2 selectionBoxTextureSize = glm::vec2((float)24/256,(float)24/64);

    glm::vec2 selectionBoxSize = glm::vec2((float)24/182*size.x,(float)24/22*size.y);
    float selectionBoxXLoc = (float)(182+2)/182*loc.x + world.getHotbarSelection()*size.x*20/182;
    glm::vec2 selectionBoxLoc = glm::vec2(selectionBoxXLoc,-1.0f+((float)23/22)*hotbarScreenHeight);


    uiTextureShaderProgramAtlas.setVec2("loc",selectionBoxLoc);
    uiTextureShaderProgramAtlas.setVec2("size",selectionBoxSize);
    uiTextureShaderProgramAtlas.setVec2("textureLoc",selectionBoxTextureLoc);
    uiTextureShaderProgramAtlas.setVec2("textureSize",selectionBoxTextureSize);
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);


    // Render the inventory contents.
    glBindVertexArray(VAO2dTextureArray);
    uiTextureShaderProgramArray.use();

    RenderableInventory renderableInventory = world.getRenderableInventory();
    float pixelsTextures = 10.0f; // Just for now. These are the number of pixels of the original hotbar texture. Not actual pixels on the screen.
    float itemCornerOffset = (20.0f-pixelsTextures)/2; // The squares in the hotbar texture have size 20x20.

    float cornerX = (float)(91-1-itemCornerOffset)/91*loc.x; // The hotbar Texture is 182 pixels wide. There is one extra column on the side.
    float cornerY = -1.0f+(float)(22-1-itemCornerOffset)/22*hotbarScreenHeight;
    glm::vec2 itemSize = glm::vec2(pixelsTextures/182*size.x,pixelsTextures/22*size.y);
    // uiTextureShaderProgram.setInt("textureAtlas",renderer.blockTextureSlotOffset);
    uiTextureShaderProgramArray.setVec2("size",itemSize);
    for(int i=0; i<9; i++){
        if(renderableInventory.slotOccupied[i]){
            uiTextureShaderProgramArray.setVec2("loc",glm::vec2(cornerX+(float)i*20/182*size.x,cornerY));
            // uiTextureShaderProgram.setVec2("textureLoc",TextureAtlas::getUVCoord(renderableInventory.slotContents[i],FaceType::Side));
            // uiTextureShaderProgram.setVec2("textureSize",TextureAtlas::getTextureSize());
            uiTextureShaderProgramArray.setFloat("textureLayer",BlockRegistry::getTextureIndex(renderableInventory.slotContents[i],FaceType::Side));
            if(renderableInventory.slotContents[i]==BlockID::Oak_Leaves){
                uiTextureShaderProgramArray.setVec4("tint",glm::vec4((float)leavesTint[0]/255.0f,(float)leavesTint[1]/255.0f,(float)leavesTint[2]/255.0f,1.0f)); 
                glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
                uiTextureShaderProgramArray.setVec4("tint",glm::vec4(1.0f,1.0f,1.0f,1.0f));
            } else if(renderableInventory.slotContents[i]==BlockID::Water){
                uiTextureShaderProgramArray.setVec4("tint",glm::vec4((float)waterTint[0]/255.0f,(float)waterTint[1]/255.0f,(float)waterTint[2]/255.0f,1.0f)); 
                glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
                uiTextureShaderProgramArray.setVec4("tint",glm::vec4(1.0f,1.0f,1.0f,1.0f));
            } else { 
                glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
            }


        }
    }
    glEnable(GL_DEPTH_TEST);
}

void UIRenderer::renderUnderwater(){
    glBindVertexArray(VAO2dTextureArray);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    
    glm::vec2 size = glm::vec2(2.0f,2.0f);
    glm::vec2 loc = glm::vec2(-1.0f,1.0f);
    
    uiTextureShaderProgramArray.use();
    // uiTextureShaderProgramArray.setInt("textureAtlas",renderer.blockTextureSlotOffset);
    uiTextureShaderProgramArray.setVec2("loc",loc);
    uiTextureShaderProgramArray.setVec2("size",size);
    // uiTextureShaderProgramArray.setVec2("textureLoc",TextureAtlas::getUVCoord(BlockID::Water,FaceType::Side));
    // uiTextureShaderProgramArray.setVec2("textureSize",TextureAtlas::getTextureSize());
    uiTextureShaderProgramArray.setFloat("textureLayer",BlockRegistry::getTextureIndex(BlockID::Water,FaceType::Side));

    uiTextureShaderProgramArray.setVec4("tint",glm::vec4((float)underwaterTint[0]/255.0f,(float)underwaterTint[1]/255.0f,(float)underwaterTint[2]/255.0f,1.0f)); 
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
    uiTextureShaderProgramArray.setVec4("tint",glm::vec4(1.0f,1.0f,1.0f,1.0f));

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

bool UIRenderer::setupRectangleRenderer(){

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
    rectangleShaderProgram = Shader("src/shaders/uiShaders/rectangleShader.vs","src/shaders/uiShaders/rectangleShader.fs");
    return true;
}

void UIRenderer::renderCrosshair(){
    glBindVertexArray(VAORectangle);
    glDisable(GL_DEPTH_TEST);
    rectangleShaderProgram.use();
    rectangleShaderProgram.setVec4("color",glm::vec4(0.3f,0.3f,0.3f,1.0f));
    int width, height;
    glfwGetWindowSize(renderer.getWindow(), &width, &height);
    float widthHeightRatio = (float)width/height; //Generally >1.
    rectangleShaderProgram.setVec2("loc",glm::vec2(-crossHairWidth/widthHeightRatio,crossHairLength));
    rectangleShaderProgram.setVec2("size",glm::vec2(2*crossHairWidth/widthHeightRatio,2*crossHairLength));
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    rectangleShaderProgram.setVec2("loc",glm::vec2(-crossHairLength/widthHeightRatio,crossHairWidth));
    rectangleShaderProgram.setVec2("size",glm::vec2(2*crossHairLength/widthHeightRatio,2*crossHairWidth));
    glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}