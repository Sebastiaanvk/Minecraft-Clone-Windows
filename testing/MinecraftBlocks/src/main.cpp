#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "../include/shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>

const int WIDTH = 1600;
const int HEIGHT = 1200;


//Global Variables:
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float yaw;
float pitch;
float fov;

float lastX, lastY;

bool firstMouse;


void framebuffer_size_callback(GLFWwindow* , int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


glm::vec2 jsonGet(const nlohmann::json& data, const std::string& textureName, int atlasWidth, int atlasHeight){
    float xLoc = ((float)data["frames"][textureName]["frame"]["x"])/((float)atlasWidth);
    float yLoc = ((float)data["frames"][textureName]["frame"]["y"])/((float)atlasHeight); 
    return {xLoc, yLoc};
}


int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,  "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glViewport(0, 0, WIDTH, HEIGHT);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);




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

    // Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    
    glBindVertexArray(VAO);


    unsigned int VBO;
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

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexSide), vertexSide, GL_STATIC_DRAW);
    // side attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);

//  Shaders:
    Shader shaderprogram = Shader("../include/shader.vs","../include/shader.fs");
    
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

    int atlasWidth, atlasHeight, nrChannels;
    unsigned char *data = stbi_load("../assets/blockAtlas.png", &atlasWidth, &atlasHeight, &nrChannels, 0); 
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shaderprogram.use();
    shaderprogram.setInt("textureAtlas",0);


    glEnable(GL_DEPTH_TEST);  


    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    int textureTypes[] = {0,1,2,3,2,3,2,3,0,1};
    float rots[]={5,0,0,10,0,0,15,0,0,30};

// Set up global variables
    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    yaw = -90.0f;
    pitch = 0.0f;
    fov = 45.0f;

    firstMouse = true;
    glfwFocusWindow(window);
    std::ifstream f("../assets/blockAtlasJson.json");
    nlohmann::json jsonData = nlohmann::json::parse(f);
    
    enum TextureTypes{
        dirt,
        gravel,
        stone,
        grassDirt
    };

    glm::vec2 textureOffsets[4][6] = {
        jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight),
        jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),jsonGet(jsonData, "gravel.png",atlasWidth,atlasHeight),
        jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),jsonGet(jsonData, "stone.png",atlasWidth,atlasHeight),
        jsonGet(jsonData, "grass_block_side.png",atlasWidth,atlasHeight),jsonGet(jsonData, "grass_block_side.png",atlasWidth,atlasHeight),jsonGet(jsonData, "grass_block_side.png",atlasWidth,atlasHeight),jsonGet(jsonData, "grass_block_side.png",atlasWidth,atlasHeight),jsonGet(jsonData, "green_concrete_powder.png",atlasWidth,atlasHeight),jsonGet(jsonData, "dirt.png",atlasWidth,atlasHeight)
    };

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(VAO);

        glm::mat4 view;
        view = glm::lookAt(cameraPos,cameraFront+cameraPos, cameraUp);

        unsigned int viewLoc = glGetUniformLocation(shaderprogram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov),  (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        unsigned int projectionLoc = glGetUniformLocation(shaderprogram.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for(unsigned int i = 0; i < 10; i++)
            {
                unsigned int textureSizeLoc = glGetUniformLocation(shaderprogram.ID, "textureSize");
                glUniform2f(textureSizeLoc,(float)16 / (float)atlasWidth,(float)16 / (float)atlasHeight);

                unsigned int offsetLoc = glGetUniformLocation(shaderprogram.ID, "textureOffset");
                glUniform2fv(offsetLoc, 6, glm::value_ptr(textureOffsets[textureTypes[i]][0]));

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i; 
                model = glm::rotate(model, glm::radians(angle)+(float)glfwGetTime() * glm::radians(rots[i]), glm::vec3(1.0f, 0.3f, 0.5f));
                shaderprogram.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f*deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
//    std::cout << "Mouse moved!\n";
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;  

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}
