#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <vector>
#include <unordered_map>
#include "../include/input_keys.hpp"



class Input_Handler {
public:
    Input_Handler();
    bool init(GLFWwindow* window);
    void update(GLFWwindow* window);
    
    bool key_down(Key k);
    bool key_pressed(Key k);
    bool key_released(Key k);

    double getDX();
    double getDY();

private:
    typedef int GLFWkey;
    double previousX, previousY;
    double dx,dy;
    double scroll_dx, scroll_dy; 
    std::unordered_map<Key,bool> prev_pressed; // All of these could be vectors or arrays, but I really dont feel like casting all the time and making the code ugly
    std::unordered_map<Key,bool> curr_pressed;
    std::unordered_map<Key,GLFWkey> toGLFWkey;
};


#endif //INPUT_HANDLER_HPP