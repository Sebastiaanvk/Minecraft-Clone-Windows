#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/renderer.hpp"
#include "../include/input_handler.hpp"
#include "../include/model.hpp"

class Game {
public:
    Game();
    void run();
private:
    Renderer renderer;
    Input_Handler input_handler;
    Model model;
};


#endif //GAME_HPP