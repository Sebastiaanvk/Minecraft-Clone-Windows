#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <input/input_keys.hpp>
#include <iostream>



class Input_Handler {
public:
    Input_Handler();
    bool init(GLFWwindow* window);
    void update(GLFWwindow* window);
    
    bool key_down(Key k);
    bool key_pressed(Key k);
    bool key_released(Key k);
    // double getScrollDiff();
    double getScrollDiffWithReset();

    double getDX();
    double getDY();

    // void setFreeMouse(bool free);
    void switchFreeMouse(GLFWwindow* window);

    void reset();


// Had to do it like this, becuase glfw is too old to accept functions that are methods of a class.
    static double scrollDiff;
private:
    typedef int GLFWkey;
    double previousX, previousY;
//    static double previousX, previousY;
//    static double tempDx, tempDy;
    double dx = 0.0f;
    double dy = 0.0f;
//    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//    double scroll_dx, scroll_dy; 
    std::unordered_map<Key,bool> prev_down; // All of these could be vectors or arrays, but I really dont feel like casting all the time and making the code ugly
    std::unordered_map<Key,bool> curr_down;
    std::unordered_map<Key,bool> pressed;
    std::unordered_map<Key,bool> released;
    std::unordered_map<Key,GLFWkey> toGLFWkey;

    bool freeMouse = false;

};


#endif //INPUT_HANDLER_HPP