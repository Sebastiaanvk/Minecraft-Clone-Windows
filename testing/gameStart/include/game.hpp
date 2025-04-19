#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/renderer.hpp"
#include "../include/input_handler.hpp"
#include "../include/world.hpp"
#include "../include/input_keys.hpp"
#include "../include/camera.hpp"

class Game {
public:
    Game();
    void run();
private:
    Renderer renderer;
    Input_Handler input_handler;
    Model model;
    Camera camera;
    void process_input();

    float deltaTime;
    float lastFrame;
};


#endif //GAME_HPP