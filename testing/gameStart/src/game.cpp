#include "../include/game.hpp"


Game::Game(){
    renderer = Renderer();
    input_handler  =  Input_Handler();
    model = Model();
    camera = Camera();
    deltaTime = 0.0f;
    lastFrame = 0.0f;
}

void Game::run(){

    if(!renderer.init(1600,1200)){
        return;
    }

    if(!input_handler.init(renderer.getWindow())){
        return;
    }

    if(!camera.init()){
        return;
    }

    while( !glfwWindowShouldClose(renderer.getWindow()) ){

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        input_handler.update(renderer.getWindow());

        process_input();
        
    }
}


void Game::process_input(){

    if(input_handler.key_pressed(Key::ESCAPE)){
        glfwSetWindowShouldClose(renderer.getWindow(), true);
    }

    if(input_handler.key_pressed(Key::FORWARD)){
        camera.move_forward(deltaTime);
    }

    if(input_handler.key_pressed(Key::BACKWARD)){
        camera.move_backward(deltaTime);
    }

    if(input_handler.key_pressed(Key::LEFT)){
        camera.move_left(deltaTime);
    }

    if(input_handler.key_pressed(Key::RIGHT)){
        camera.move_right(deltaTime);
    }

    camera.rotate(input_handler.getDX(),input_handler.getDY());
}