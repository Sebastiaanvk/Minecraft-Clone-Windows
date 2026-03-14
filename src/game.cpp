#include <game.hpp>


Game::Game()
    :renderer(),
    input_handler(),
    seed(std::chrono::system_clock::now().time_since_epoch().count()),
    world(seed),
    camera()
{
    std::cout << "Seed: "<< seed << std::endl;

}



void Game::run(){

    if(!renderer.init(1600,1000)){
        return;
    }


    if(!input_handler.init(renderer.getWindow())){
        return;
    }

    while( !glfwWindowShouldClose(renderer.getWindow()) ){

        float currentFrame = glfwGetTime();
        nrFramesInSecond += 1;
        if(currentFrame-lastSecondFrame>1.0f){
            frameRate = (float)nrFramesInSecond/(currentFrame-lastSecondFrame);
            lastSecondFrame = currentFrame;
            nrFramesInSecond = 0;
        }
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        glfwPollEvents();    
        input_handler.update(renderer.getWindow());

        process_input();
        timeAccumulator += deltaTime;
        while(timeAccumulator>=world.tickTimeLength){
            if(!paused){
                START_TIMING(worldUpdate)
                world.setFrustumSettings(renderer.renderSettings.projectionNearDistance,renderer.renderSettings.projectionFarDistance,camera.getFov(),(float)camera.getFov()/renderer.getAspectRatio());
                world.update(input_handler);        
                END_TIMING(worldUpdate)
            }
            timeAccumulator -= world.tickTimeLength;
        }

        float alpha = timeAccumulator/world.tickTimeLength;

        camera.update(world.player,alpha);
        
        START_TIMING(render)
        renderer.render(world,camera,getGameUIData());
        END_TIMING(render)

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
    world.player.rotate(input_handler.getDX(),input_handler.getDY());
}

GameUIData Game::getGameUIData(){
        GameUIData gameData;
        gameData.paused = paused;
        gameData.frameRate = frameRate;
        return gameData;
}