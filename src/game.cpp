#include <game.hpp>


Game::Game()
    :renderer(),
    input_handler(),
    seed(std::chrono::system_clock::now().time_since_epoch().count()),
    world(seed),
    camera(),
    deltaTime(0.0f),
    lastFrame(0.0f)
{
    // std::mt19937 generator(seed); // Mersenne Twister engine (Not sure why this is here)
    std::cout << "Seed: "<< seed << std::endl;
}



void Game::run(){

    if(!renderer.init(2200,2000)){
        return;
    }
    glfwSwapInterval(1); // Enables VSync


    if(!input_handler.init(renderer.getWindow())){
        return;
    }

    if(!camera.init()){
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

        renderer.render(world,camera);

        // std::cout<< camera.cameraStatsAsString() << std::endl;

    }
    glfwTerminate();
}


void Game::process_input(){

    if(input_handler.key_pressed(Key::ESCAPE)){
        glfwSetWindowShouldClose(renderer.getWindow(), true);
    }
    if(input_handler.key_down(Key::FORWARD)){
        camera.move_forward(deltaTime);
    }

    if(input_handler.key_down(Key::BACKWARD)){
        camera.move_backward(deltaTime);
    }

    if(input_handler.key_down(Key::LEFT)){
        camera.move_left(deltaTime);
    }

    if(input_handler.key_down(Key::RIGHT)){
        camera.move_right(deltaTime);
    }
    if(input_handler.key_down(Key::SPACE)){
        camera.move_up(deltaTime);
    }
    if(input_handler.key_down(Key::LEFT_SHIFT)){
        camera.move_down(deltaTime);
    }
    // std::cout << "dx: " << input_handler.getDX() << " dy: " << input_handler.getDY() << "\n";
    camera.rotate(input_handler.getDX(),input_handler.getDY());
}