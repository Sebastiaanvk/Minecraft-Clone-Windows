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

std::vector<float> Renderer::updateVBOVector(const RenderableChunkMesh& worldMesh){
    std::vector<float> vertices;
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
            int corner = cornerOrder[i];
            vertices.push_back(face.corners[corner].x);
            vertices.push_back(face.corners[corner].y);
            vertices.push_back(-face.corners[corner].z);
            vertices.push_back(uvCoord.x+uvDiff[corner].first*textureSizeWidth);
            vertices.push_back(uvCoord.y+uvDiff[corner].second*textureSizeHeight);
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
    std::vector<float> vertices = updateVBOVector(worldMesh);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    renderMesh.nrVertices = worldMesh.mesh.size()*6;
    // Position attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    return renderMesh;
}

// This is just for testing, delete later!
void setupTestMeshes( int atlasWidth, int atlasHeight){
    glGenVertexArrays(1,&testVAO);
    glBindVertexArray(testVAO);

    glGenBuffers(1,&testVBO);
    glBindBuffer(GL_ARRAY_BUFFER, testVBO);

    float atlasRatio = (float) atlasHeight/atlasWidth;
    
    float testVertices [] = {
        -5,5-10*atlasRatio,-5,0,1,
        5,5,-5,1,0,
        -5,5,-5,0,0,
        5,5,-5,1,0,
        -5,5-10*atlasRatio,-5,0,1,
        5,5-10*atlasRatio,-5,1,1
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

}

bool Renderer::init(int width, int height){
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


//  Shaders:
     shaderprogram = Shader("include/shaders/chunkShader.vs","include/shaders/chunkShader.fs");
    //  shaderprogram = Shader("../include/shaders/chunkShader.vs","../include/shaders/chunkShader.fs");
    
//    stbi_set_flip_vertically_on_load(true);  

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    stbi_image_free(data);

    shaderprogram.use();
    shaderprogram.setInt("textureAtlas",0);

    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);

    std::ifstream f("assets/blockAtlasJson.json");
    jsonAtlasData = nlohmann::json::parse(f);

    setupTestMeshes(atlasWidth, atlasHeight);


    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}






void Renderer::render(World& world, Camera& camera){

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    view = camera.getViewMatrix();

    unsigned int viewLoc = glGetUniformLocation(shaderprogram.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.getFov()),  (float)width / (float)height, 0.1f, 100.0f);

    unsigned int projectionLoc = glGetUniformLocation(shaderprogram.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    std::queue<std::shared_ptr<RenderableChunkMesh>> chunkQueue = world.toRenderableChunkQueue();
    while(!chunkQueue.empty()){
        std::shared_ptr<RenderableChunkMesh> chunkPtr = chunkQueue.front();
        chunkQueue.pop();
        ChunkID chunkID = chunkPtr->chunkId;
        if(chunkMeshes.count(chunkID)==0){
            chunkMeshes[chunkID] = createRenderMesh(*chunkPtr);
            glBindVertexArray(chunkMeshes[chunkID].VAO);
        } else if(chunkPtr->updated){
            std::vector<float> vertices = updateVBOVector(*chunkPtr);
            chunkMeshes[chunkID].nrVertices = 6*chunkPtr->mesh.size();
            glBindVertexArray(chunkMeshes[chunkID].VAO);
            glBindBuffer(GL_ARRAY_BUFFER,chunkMeshes[chunkID].VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(float), vertices.data());

            chunkPtr->updated = false;
        } else {
            glBindVertexArray(chunkMeshes[chunkID].VAO);
        }

        glDrawArrays(GL_TRIANGLES, 0, chunkMeshes[chunkID].nrVertices);
    }

    glBindVertexArray(testVAO);
    glDrawArrays(GL_TRIANGLES,0,6);


    glfwSwapBuffers(window);
}