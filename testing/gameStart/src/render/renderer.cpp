#include <render/renderer.hpp>


//This will get Fixed!!!
// just want to render some cubes for testing!!!
//cube Vertices
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float vertexSide[] = {
    0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
    1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
    2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,
    3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,
    4.0f,4.0f,4.0f,4.0f,4.0f,4.0f,
    5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,
    6.0f,6.0f,6.0f,6.0f,6.0f,6.0f
};
Renderer::Renderer(){

}


void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
} 

void Renderer::setUpBuffers(){


    // Vertex Array Object
    glGenVertexArrays(1, &VAO);  
    
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    //  Vertex attributes:
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexSide), vertexSide, GL_STATIC_DRAW);
    // side attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);
}

bool Renderer::init(int width, int height){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width,height,  "LearnOpenGL", NULL, NULL);
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
     shaderprogram = Shader("../include/shaders/shader.vs","../include/shaders/shader.fs");
    
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
    unsigned char *data = stbi_load("../assets/blockAtlas.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
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

    std::ifstream f("../assets/blockAtlasJson.json");
    jsonAtlasData = nlohmann::json::parse(f);

//Maybe add the option to scroll with a callback?

    setUpBuffers();


    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}

glm::vec2 jsonGet(const nlohmann::json& data, const std::string& textureName, int atlasWidth, int atlasHeight){
    float xLoc = ((float)data["frames"][textureName]["frame"]["x"])/((float)atlasWidth);
    float yLoc = ((float)data["frames"][textureName]["frame"]["y"])/((float)atlasHeight); 
    return {xLoc, yLoc};
}

void Renderer::render(World& world, Camera& camera){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindVertexArray(VAO);

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

    std::queue<Renderable>  renQ = world.toRenderableQueue();
    //This while loop is hella sketchy. Will change later!!!!
    while(!renQ.empty())
        {
            Renderable ren = renQ.front();
            renQ.pop();

            unsigned int textureSizeLoc = glGetUniformLocation(shaderprogram.ID, "textureSize");
            glUniform2f(textureSizeLoc,(float)16 / (float)atlasWidth,(float)16 / (float)atlasHeight);

            
            glm::vec2 textureOffsets[6] = {jsonGet(jsonAtlasData, ren.sideTexture,atlasWidth,atlasHeight),jsonGet(jsonAtlasData, ren.sideTexture,atlasWidth,atlasHeight),jsonGet(jsonAtlasData, ren.sideTexture,atlasWidth,atlasHeight),jsonGet(jsonAtlasData, ren.sideTexture,atlasWidth,atlasHeight),jsonGet(jsonAtlasData, ren.topTexture,atlasWidth,atlasHeight),jsonGet(jsonAtlasData, ren.botTexture,atlasWidth,atlasHeight)};

            unsigned int offsetLoc = glGetUniformLocation(shaderprogram.ID, "textureOffset");
            glUniform2fv(offsetLoc, 6, glm::value_ptr(textureOffsets[0]));

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(ren.x,ren.y,ren.z) );
            shaderprogram.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

    glfwSwapBuffers(window);

}