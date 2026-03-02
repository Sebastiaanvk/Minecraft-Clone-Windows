#include <world/world.hpp>



// World::World(){

// }

World::World(unsigned int seed)
    : chunkManager(seed),player(),
    blockTargeted(false),tick(0)
{
}



std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    return chunkManager.toRenderableChunkQueue();
}


void World::calculatePlayerTarget() {
    /* Idea:
    We look for the first cube surface we intersect with a ray going out of the camera.
    There are 6 different sides to a block, so we need to see which side gets intersected first by the ray.
    Hence, the split for x,y,z en whether the direction in that dimension is positive or negative.
    
    */
    float maxReach = 12;
    LocFloat pos = player.getPos();
    LocFloat forward = player.getForwardDir();

    float distSolidX = 20*maxReach;
    LocInt closestSolidX;
    LocInt placementCandidateX;

    if(forward.x>0){
        float nextX = ceil(pos.x);
        // We know the forward has a length of 1, so we can easily calculate the distance the ray needs to travel in order to intersect the plane given by x==nextX.
        float dist = ((nextX-pos.x)/ forward.x);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x+0.5f),(int)floor(intersect.y),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidX = blockCandidate;
                placementCandidateX = blockCandidate + LocInt{-1,0,0};
                distSolidX = dist;
                break;
            }
            nextX += 1;
            dist = (((float)nextX-pos.x)/ forward.x);
        }
    } else if(forward.x<0){
        float nextX = floor(pos.x);
        float dist = ((nextX-pos.x)/ forward.x);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x-0.5f),(int)floor(intersect.y),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidX = blockCandidate;
                placementCandidateX = blockCandidate + LocInt{1,0,0};
                distSolidX = dist;
                break;
            }
            nextX -= 1;
            dist = (((float)nextX-pos.x)/ forward.x);
        }
    }

    float distSolidY = 20*maxReach;
    LocInt closestSolidY;
    LocInt placementCandidateY;

    if(forward.y>0){
        float nextY = ceil(pos.y);
        float dist = ((nextY-pos.y)/ forward.y);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y+0.5f),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidY = blockCandidate;
                placementCandidateY = blockCandidate + LocInt{0,-1,0};
                distSolidY = dist;
                break;
            }
            nextY += 1;
            dist = (((float)nextY-pos.y)/ forward.y);
        }
    } else if(forward.y<0){
        float nextY = floor(pos.y);
        float dist = ((nextY-pos.y)/ forward.y);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y-0.5f),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidY = blockCandidate;
                placementCandidateY = blockCandidate + LocInt{0,1,0};
                distSolidY = dist;
                break;
            }
            nextY -= 1;
            dist = (((float)nextY-pos.y)/ forward.y);
        }
    }

    float distSolidZ = 20*maxReach;
    LocInt closestSolidZ;
    LocInt placementCandidateZ;

    if(forward.z>0){
        float nextZ = ceil(pos.z);
        float dist = ((nextZ-pos.z)/ forward.z);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y),(int)floor(intersect.z+0.5f)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidZ = blockCandidate;
                placementCandidateZ = blockCandidate + LocInt{0,0,-1};
                distSolidZ = dist;
                break;
            }
            nextZ += 1;
            dist = (((float)nextZ-pos.z)/ forward.z);
        }
    } else if(forward.z<0){
        float nextZ = floor(pos.z);
        float dist = ((nextZ-pos.z)/ forward.z);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y),(int)floor(intersect.z-0.5f)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidZ = blockCandidate;
                placementCandidateZ = blockCandidate + LocInt{0,0,1};
                distSolidZ = dist;
                break;
            }
            nextZ -= 1;
            dist = (((float)nextZ-pos.z)/ forward.z);
        }
    }
    float minDist;
    LocInt closestSolid;
    if(distSolidX<distSolidY){
        minDist = distSolidX;
        closestSolid = closestSolidX;
        placementCandidate = placementCandidateX;
    } else {
        minDist = distSolidY;
        closestSolid = closestSolidY;
        placementCandidate = placementCandidateY;
    }
    if(distSolidZ<minDist){
        minDist = distSolidZ;
        closestSolid = closestSolidZ;
        placementCandidate = placementCandidateZ;
    }
    if(minDist < maxReach){
        blockTargeted = true;
        targetedBlock = closestSolid;
    } else {
        blockTargeted = false;
    }

}

void World::deleteTarget(){
    // std::cout << "Deleting target. BlockTargeted: " << blockTargeted << ", Location: " << targetedBlock.x << "," << targetedBlock.y << "," << targetedBlock.z << std::endl;  
    static int lastDeletionTick = -ticksBetweenBlockManipulation;
    
    if( tick-lastDeletionTick>=ticksBetweenBlockManipulation and blockTargeted){
        lastDeletionTick = tick;
        chunkManager.deleteBlock(targetedBlock);
    }

}

void World::placeBlock(){
    static int lastPlacementTick = -ticksBetweenBlockManipulation;
    if( tick-lastPlacementTick>=ticksBetweenBlockManipulation and blockTargeted and !player.blockIntersects(placementCandidate)){
        chunkManager.placeBlock(placementCandidate);
        lastPlacementTick = tick;

    }
}

void World::update(Input_Handler& input_handler){

    tick += 1;

    player.storePos();

    if(input_handler.key_down(Key::FORWARD)){
        player.move_forward(tickTimeLength);
    }

    if(input_handler.key_down(Key::BACKWARD)){
        player.move_backward(tickTimeLength);
    }

    if(input_handler.key_down(Key::LEFT)){
        player.move_left(tickTimeLength);
    }

    if(input_handler.key_down(Key::RIGHT)){
        player.move_right(tickTimeLength);
    }
    if(input_handler.key_down(Key::SPACE)){
        player.move_up(tickTimeLength);
    }
    if(input_handler.key_down(Key::LEFT_SHIFT)){
        player.move_down(tickTimeLength);
    }
    if(input_handler.key_down(Key::LEFT_MOUSE_BUTTON)){
        deleteTarget();
    } else if(input_handler.key_down(Key::RIGHT_MOUSE_BUTTON)){
        placeBlock();
    }
    calculatePlayerTarget();
    // std::cout << "BlockTargeted: " << blockTargeted << ", Location: " << targetedBlock.x << "," << targetedBlock.y << "," << targetedBlock.z << std::endl;  

}


WorldUIData World::getUIData(){
    WorldUIData uiData;
    uiData.playerData = player.getUIData();
    uiData.blockTargeted = blockTargeted;
    uiData.targetedBlock = targetedBlock;
    uiData.tick = tick;
    uiData.ticksBetweenBlockManipulationP = &ticksBetweenBlockManipulation;
    uiData.tickTimeLengthP = &tickTimeLength;
    return uiData;
}




