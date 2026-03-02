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
            if(!paused){
                world.update(input_handler);        
            }
            timeAccumulator -= world.tickTimeLength;
        }


        float alpha = timeAccumulator/world.tickTimeLength;

        camera.update(world.player,alpha);

        GameUIData gameData;
        gameData.paused = paused;
        renderer.render(world,camera,gameData);

        // std::cout<< world.player.playerLocAsString() << std::endl;

    }
    renderer.shutDown();
    glfwTerminate();
}


void Game::process_input(){

    if(input_handler.key_pressed(Key::ESCAPE)){
        glfwSetWindowShouldClose(renderer.getWindow(), true);
    }
    if(input_handler.key_pressed(Key::PAUSE)){
        input_handler.switchFreeMouse(renderer.getWindow());
        paused = !paused;
        if(!paused){
            input_handler.reset();
        }
    }
    // std::cout << "dx: " << input_handler.getDX() << " dy: " << input_handler.getDY() << "\n";
    world.player.rotate(input_handler.getDX(),input_handler.getDY());
}


