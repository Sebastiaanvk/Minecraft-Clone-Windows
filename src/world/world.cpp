#include <world/world.hpp>



World::World(unsigned int seed)
    : chunkManager(seed),player()
{
    // START_TIMING(Terrain)
    chunkManager.generateTerrains(player.getBlockLoc(),10);
    // END_TIMING(Terrain)
    // START_TIMING(Trees)
    chunkManager.generateTrees(player.getBlockLoc(),9);
    // END_TIMING(Trees)
    // START_TIMING(Meshes)
    chunkManager.calculateMeshes(player.getBlockLoc(),8);
    // END_TIMING(Meshes)
}


void World::update(Input_Handler& input_handler){

    tick += 1;
    updatePlayerLocation(input_handler);
    
    START_TIMING(generateTerrain)
    chunkManager.generateTerrainsAsync(player.getBlockLoc() );
    END_TIMING(generateTerrain)
    START_TIMING(generateTrees)
    chunkManager.generateTreesAsync(player.getBlockLoc());
    END_TIMING(generateTrees)
    START_TIMING(generateFrustumCull)
    frustumCull();
    END_TIMING(generateFrustumCull)
    START_TIMING(calculateMeshesAsync)
    chunkManager.calculateMeshesAsync(player.getBlockLoc());
    END_TIMING(calculateMeshesAsync)

    if(input_handler.key_down(Key::LEFT_MOUSE_BUTTON)){
        deleteTarget();
    } else if(input_handler.key_down(Key::RIGHT_MOUSE_BUTTON)){
        placeBlock();
    }
    float scrollDiff = input_handler.getScrollDiffWithReset();
    if(scrollDiff>0.1f){
        player.changeHotbarSelection(true);
    } else if( scrollDiff<-0.1f){
        player.changeHotbarSelection(false);
    }

    calculatePlayerTarget();
    // chunkManager.generateChunks(player.getBlockLoc() );

    START_TIMING(calcMeshes)
    chunkManager.calculateMeshes(player.getBlockLoc());
    END_TIMING(calcMeshes)
}


void World::setFrustumSettings(float renderDistanceNear, float renderDistanceFar, float fovX, float fovY){
    frustumCullingPars.near = renderDistanceNear;
    frustumCullingPars.far = renderDistanceFar;
    frustumCullingPars.fovX = fovX;
    frustumCullingPars.fovY = fovY;
}


glm::vec3 rotate(const glm::vec3& dir,float degrees, const glm::vec3& axis){
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(degrees), axis);
    return glm::vec3(rotationMatrix * glm::vec4(dir,1.0f));
}

void World::frustumCull(){
    frustumCullingPars.cameraLoc = player.getPos();
    frustumCullingPars.forward = player.getForwardDir();
    // float pitch = player.getPitch();
    // float yaw = player.getYaw();
    glm::vec3 right = glm::normalize(glm::cross(frustumCullingPars.forward, {0,1,0}));
    glm::vec3 up = glm::normalize(glm::cross(right, frustumCullingPars.forward));
    float fovX = frustumCullingPars.fovX;
    float fovY = frustumCullingPars.fovY;
    // float fovX = 45.0f; // Just for testing purposes
    // float fovY = 45.0f; // Just for testing purposes
    // frustumCullingPars.left = yawPitchToVector(yaw+90-fovX/2,pitch);//Double check if the directions of the degrees are correct like this!
    // frustumCullingPars.right = yawPitchToVector(yaw-90+fovX/2,pitch);
    // frustumCullingPars.top = yawPitchToVector(yaw,pitch+90-fovY/2);
    // frustumCullingPars.bottom = yawPitchToVector(yaw,pitch-90+fovY/2);
    // frustumCullingPars.normals[0] = yawPitchToVector(yaw+90,pitch);//Double check if the directions of the degrees are correct like this!
    // frustumCullingPars.normals[1] = yawPitchToVector(yaw-90,pitch);
    // frustumCullingPars.normals[2] = yawPitchToVector(yaw,pitch+90-fovY/2);
    // frustumCullingPars.normals[3] = yawPitchToVector(yaw,pitch-90+fovY/2);
    frustumCullingPars.normals[0] = rotate(frustumCullingPars.forward,90.0f-fovX/2,up);//Double check if the directions of the degrees are correct like this!
    frustumCullingPars.normals[1] = rotate(frustumCullingPars.forward,-90.0f+fovX/2,up);
    frustumCullingPars.normals[2] = rotate(frustumCullingPars.forward,90.0f-fovY/2,right);
    frustumCullingPars.normals[3] = rotate(frustumCullingPars.forward,-90.0f+fovY/2,right);
    chunkManager.cullChunks(frustumCullingPars);
}


void World::updatePlayerLocation(Input_Handler& input_handler){
    player.storePos(); // For interpolation of location between ticks.

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
}


std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    return chunkManager.toRenderableChunkQueue(player.getBlockLoc());
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
            if(chunkManager.notAir(blockCandidate)){
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
            if(chunkManager.notAir(blockCandidate)){
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
            if(chunkManager.notAir(blockCandidate)){
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
            if(chunkManager.notAir(blockCandidate)){
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
            if(chunkManager.notAir(blockCandidate)){
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
            if(chunkManager.notAir(blockCandidate)){
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
        if(player.placeableBlockSelected()){
            chunkManager.placeBlock(placementCandidate,player.getSelectedBlock());
            lastPlacementTick = tick;
        }
    }
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
RenderableInventory World::getRenderableInventory() const{
    RenderableInventory renderableInventory;

    const std::array<InventorySlot,9>& hotbarInventory =  player.getHotbar();
    for(int i=0; i<9; i++){
        renderableInventory.slotOccupied[i] = hotbarInventory[i].occupied;
        renderableInventory.slotContents[i] = hotbarInventory[i].contents;
    }
    return renderableInventory;
}


bool World::hasBlockTargeted() const{
    return blockTargeted;
}
LocInt World::getTargetedBlock() const{
    return targetedBlock;
}

bool World::playerIsUnderwater() const {
    return chunkManager.isUnderwater(player.getBlockLoc());
}

int World::getHotbarSelection() const{
    return player.getHotbarSelectionIndex();
}

