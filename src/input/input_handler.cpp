#include <input/input_handler.hpp>



Input_Handler::Input_Handler(){
    toGLFWkey[Key::ESCAPE] = GLFW_KEY_ESCAPE;
    toGLFWkey[Key::FORWARD] = GLFW_KEY_W;
    toGLFWkey[Key::BACKWARD] = GLFW_KEY_S;
    toGLFWkey[Key::LEFT] = GLFW_KEY_A;
    toGLFWkey[Key::RIGHT] = GLFW_KEY_D;
    toGLFWkey[Key::SPACE] = GLFW_KEY_SPACE;
    toGLFWkey[Key::LEFT_SHIFT] = GLFW_KEY_LEFT_SHIFT;
    toGLFWkey[Key::LEFT_MOUSE_BUTTON] = GLFW_MOUSE_BUTTON_LEFT,
    toGLFWkey[Key::RIGHT_MOUSE_BUTTON] = GLFW_MOUSE_BUTTON_RIGHT;
    toGLFWkey[Key::PAUSE] = GLFW_KEY_P;

    prev_down = {};
    curr_down = {};
    for(int i = 0; i<Key::KEY_COUNT; i++){ // Technically unnecessary, but I wanted to prevent bugs where the entry of the map is read
        Key k = static_cast<Key>(i);
        prev_down[k] = false;
        curr_down[k] = false;
        pressed[k] = false;
        released[k] = false;
    }
}


bool Input_Handler::init(GLFWwindow* window){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    previousX = ((double)width)/2;
    previousY = ((double)height)/2;
    dx = 0.0f;
    dy = 0.0f;
    glfwSetCursorPos(window, previousX , previousY);
//    glfwSetCursorPosCallback(window, mouse_callback);  
    return true;
}



void Input_Handler::update(GLFWwindow* window){
    for(int i = 0; i<Key::KEY_COUNT; i++){
        Key k = static_cast<Key>(i);
        prev_down[k] = curr_down[k];
        if( k== Key::LEFT_MOUSE_BUTTON || k==Key::RIGHT_MOUSE_BUTTON){
            curr_down[k] = (glfwGetMouseButton(window, toGLFWkey[k]) == GLFW_PRESS);
        } else {
            curr_down[k] = (glfwGetKey(window, toGLFWkey[k]) == GLFW_PRESS);
        }
        if (curr_down[k] && !(prev_down[k])){
            pressed[k] = true;
        }
        if (!curr_down[k] && prev_down[k]){
            released[k] = true;
        }
    }

    if(!freeMouse){
        double currentX,currentY; 
        glfwGetCursorPos(window, &currentX, &currentY);
        // Need to check: Does the speed of the camera depend on the window size?
        // Preferably, it should only depend on the actual mouse movement.
        dx = currentX - previousX;
        dy =  previousY - currentY; // Reversed because in GLFW the y starts at the bottom
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glfwSetCursorPos(window, ((double)width)/2, ((double)height)/2);
       
        previousX = ((double)width)/2;
        previousY = ((double)height)/2;
    }

}
bool Input_Handler::key_down(Key k){
    if (pressed[k]){
        pressed[k] = false;
        return true;
    }
    return curr_down[k];
}

bool Input_Handler::key_pressed(Key k){
    if (pressed[k]){
        pressed[k] = false;
        return true;
    }
    return false;
}

bool Input_Handler::key_released(Key k){
    if (released[k]){
        released[k] = false;
        return true;
    }
    return false;
}


double Input_Handler::getDX(){
    return dx;
}
double Input_Handler::getDY(){
    return dy;
}

// void Input_Handler::setFreeMouse(bool free){
//     freeMouse = free;
// }

void Input_Handler::switchFreeMouse(GLFWwindow* window){
    freeMouse = !freeMouse;
    if(freeMouse){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        dx = 0;
        dy = 0;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glfwSetCursorPos(window, ((double)width)/2, ((double)height)/2);
       
        previousX = ((double)width)/2;
        previousY = ((double)height)/2;
    }
}

void Input_Handler::reset(){
    for(int i = 0; i<Key::KEY_COUNT; i++){ // Technically unnecessary, but I wanted to prevent bugs where the entry of the map is read
        Key k = static_cast<Key>(i);
        prev_down[k] = false; 
        curr_down[k] = false;
        pressed[k] = false;
        released[k] = false;
    }
    curr_down[Key::PAUSE] = true; //Important in order to prevent instant repausing.
}

/*
void Input_Handler::mouse_callback(GLFWwindow* window, double xpos, double ypos){
//{
    //Maybe add this, to prevent a big jump at the start.
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
}

    tempDx += xpos - previousX;
    tempDy += previousY - ypos; // reversed since y-coordinates range from bottom to top
    previousX = xpos;
    previousY = ypos;


    std::cout << "Mouse callback\n ";
}
*/