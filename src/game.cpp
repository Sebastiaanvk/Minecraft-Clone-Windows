#include <game.hpp>


Game::Game()
    :renderer(),
    input_handler(),
    seed(std::chrono::system_clock::now().time_since_epoch().count()),
    world(seed),
    camera(),
    deltaTime(0.0f),
    lastFrame(0.0f),
    timeAccumulator(0.0f)
{
    // std::mt19937 generator(seed); // Mersenne Twister engine (Not sure why this is here)
    std::cout << "Seed: "<< seed << std::endl;
}



void Game::run(){

    if(!renderer.init(1600,1200)){
        return;
    }
    glfwSwapInterval(1); // Enables VSync


    if(!input_handler.init(renderer.getWindow())){
        return;
    }

    while( !glfwWindowShouldClose(renderer.getWindow()) ){

//        std::this_thread::sleep_for(std::chrono::milliseconds(13)); // This might cause input problems!
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        glfwPollEvents();    
        input_handler.update(renderer.getWindow());

        process_input();
        timeAccumulator += deltaTime;
        while(timeAccumulator>=world.tickTimeLength){
            world.update(input_handler);        
            timeAccumulator -= world.tickTimeLength;
        }


        float alpha = timeAccumulator/world.tickTimeLength;

        camera.update(world.player,alpha);

        renderer.render(world,camera);

        std::cout<< world.player.playerLocAsString() << std::endl;

    }
    glfwTerminate();
}


void Game::process_input(){

    if(input_handler.key_pressed(Key::ESCAPE)){
        glfwSetWindowShouldClose(renderer.getWindow(), true);
    }
    // if(input_handler.key_down(Key::FORWARD)){
    //     world.player.move_forward(deltaTime);
    // }

    // if(input_handler.key_down(Key::BACKWARD)){
    //     world.player.move_backward(deltaTime);
    // }

    // if(input_handler.key_down(Key::LEFT)){
    //     world.player.move_left(deltaTime);
    // }

    // if(input_handler.key_down(Key::RIGHT)){
    //     world.player.move_right(deltaTime);
    // }
    // if(input_handler.key_down(Key::SPACE)){
    //     world.player.move_up(deltaTime);
    // }
    // if(input_handler.key_down(Key::LEFT_SHIFT)){
    //     world.player.move_down(deltaTime);
    // }
    // if(input_handler.key_pressed(Key::LEFT_MOUSE_BUTTON)){
    //     std::cout << "Left Mouse Button Pressed!" << std::endl;
    //     world.deleteTarget();
    // }
    // if(input_handler.key_pressed(Key::RIGHT_MOUSE_BUTTON)){
    //     std::cout << "Right Mouse Button Pressed!" << std::endl;
    //     world.placeBlock();
    // }
    // if(input_handler.key_pressed(Key::SPACE)){
    //     std::cout << "Space Pressed!" << std::endl;
    // }
    // std::cout << "dx: " << input_handler.getDX() << " dy: " << input_handler.getDY() << "\n";
    world.player.rotate(input_handler.getDX(),input_handler.getDY());
}