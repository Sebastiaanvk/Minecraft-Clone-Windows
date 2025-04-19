#ifndef GAME_HPP
#define GAME_HPP

#include <external/glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <render/renderer.hpp>
#include <input/input_handler.hpp>
#include <world/world.hpp>
#include <input/input_keys.hpp>
#include <camera.hpp>

class Game {
public:
    Game();
    void run();
private:
    Renderer renderer;
    Input_Handler input_handler;
    World world;
    Camera camera;
    void process_input();

    float deltaTime;
    float lastFrame;
};


#endif //GAME_HPP