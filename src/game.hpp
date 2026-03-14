#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <render/renderer.hpp>
#include <input/input_handler.hpp>
#include <world/world.hpp>
#include <input/input_keys.hpp>
#include <render/uiData.hpp>
#include <camera.hpp>
#include <thread>
#include <chrono>
#include <random>
#include <queue>

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
    unsigned int seed;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float timeAccumulator = 0.0f;
    
    bool paused = false;

    float frameRate = -1.0f;
    float lastSecondFrame=0.0f;
    int nrFramesInSecond = 0;

    GameUIData getGameUIData();
};


#endif //GAME_HPP