#include <render/renderer.hpp>

Renderer::Renderer(){

}


void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
    glViewport(0, 0, width, height);
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }    

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

//Maybe add the option to scroll with a callback?

//TODO Set up all the vaos, vbos, what have you.


    return true;
}

GLFWwindow* Renderer::getWindow(){
    return window;
}

void Renderer::render(World& world, Camera& camera){
    /*
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(VAO);

        glm::mat4 view;
        view = camera.getViewMatrix();

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
    */

}