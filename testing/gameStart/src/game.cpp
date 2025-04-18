#include "../include/game.hpp"


Game::Game(){
    renderer = Renderer();
    input_handler  =  Input_Handler();
    model = Model();
}

void Game::run(){

    if(!renderer.init(1200,900)){
        return;
    }
    if(!input_handler.set_up_callbacks(renderer.getWindow())){
        return;
    }

    while( !glfwWindowShouldClose(renderer.getWindow()) ){




    }
    
}