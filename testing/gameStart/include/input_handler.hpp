#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <glad/glad.h>
#include "GLFW/glfw3.h"

class Input_Handler {
public:
    Input_Handler();
    bool set_up_callbacks(GLFWwindow* window);
};


#endif //INPUT_HANDLER_HPP