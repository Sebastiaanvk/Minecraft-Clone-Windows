#include <input/input_handler.hpp>



Input_Handler::Input_Handler(){
    toGLFWkey[Key::ESCAPE] = GLFW_KEY_ESCAPE;
    toGLFWkey[Key::FORWARD] = GLFW_KEY_W;
    toGLFWkey[Key::BACKWARD] = GLFW_KEY_S;
    toGLFWkey[Key::LEFT] = GLFW_KEY_A;
    toGLFWkey[Key::RIGHT] = GLFW_KEY_D;
    toGLFWkey[Key::UP] = GLFW_KEY_SPACE;
    toGLFWkey[Key::DOWN] = GLFW_KEY_LEFT_SHIFT;

    prev_pressed = {};
    curr_pressed = {};
    for(int i = 0; i<Key::KEY_COUNT; i++){ // Technically unnecessary, but I wanted to prevent bugs where the entry of the map is read
        Key k = static_cast<Key>(i);
        prev_pressed[k] = false;
        curr_pressed[k] = false;
    }
}

bool Input_Handler::init(GLFWwindow* window){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    previousX = ((double)width)/2;
    previousY = ((double)height)/2;
    glfwSetCursorPos(window, previousX , previousY);
    return true;
}



void Input_Handler::update(GLFWwindow* window){
    for(int i = 0; i<Key::KEY_COUNT; i++){
        Key k = static_cast<Key>(i);
        prev_pressed[k] = curr_pressed[k];
        curr_pressed[k] = (glfwGetKey(window, toGLFWkey[k]) == GLFW_PRESS);
    }


    double currentX,currentY; 
    glfwGetCursorPos(window, &currentX, &currentY);
    dx = currentX - previousX;
    dy = currentY - previousY ;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glfwSetCursorPos(window, ((double)width)/2, ((double)height)/2);
    
}
bool Input_Handler::key_down(Key k){
    return curr_pressed[k];
}

bool Input_Handler::key_pressed(Key k){
    return curr_pressed[k] && !(prev_pressed[k]);
}

bool Input_Handler::key_released(Key k){
    return prev_pressed[k] && !(curr_pressed[k]);
}


double Input_Handler::getDX(){
    return dx;
}
double Input_Handler::getDY(){
    return dy;
}