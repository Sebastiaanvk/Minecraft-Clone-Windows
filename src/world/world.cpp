#include <world/world.hpp>



// World::World(){

// }

World::World(unsigned int seed)
    : chunkManager(seed),player(),
    blockTargeted(false)
{
}



std::queue<std::shared_ptr<RenderableChunkMesh>> World::toRenderableChunkQueue(){
    return chunkManager.toRenderableChunkQueue();
}


void World::calculatePlayerTarget() {
    float maxReach = 10;
    LocFloat pos = player.getPos();
    LocFloat forward = player.getForwardDir();

    float distSolidX = 20*maxReach;
    LocInt closestSolidX;

    if(forward.x>0){
        float nextX = ceil(pos.x);
        float dist = ((nextX-pos.x)/ forward.x);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x+0.5f),(int)floor(intersect.y),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidX = blockCandidate;
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
                distSolidX = dist;
                break;
            }
            nextX -= 1;
            dist = (((float)nextX-pos.x)/ forward.x);
        }
    }

    float distSolidY = 20*maxReach;
    LocInt closestSolidY;

    if(forward.y>0){
        float nextY = ceil(pos.y);
        float dist = ((nextY-pos.y)/ forward.y);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y+0.5f),(int)floor(intersect.z)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidY = blockCandidate;
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
                distSolidY = dist;
                break;
            }
            nextY -= 1;
            dist = (((float)nextY-pos.y)/ forward.y);
        }
    }

    float distSolidZ = 20*maxReach;
    LocInt closestSolidZ;

    if(forward.z>0){
        float nextZ = ceil(pos.z);
        float dist = ((nextZ-pos.z)/ forward.z);
        while(  dist <= maxReach ){
            LocFloat intersect = pos + dist*forward;
            LocInt blockCandidate = {(int)floor(intersect.x),(int)floor(intersect.y),(int)floor(intersect.z+0.5f)};
            if(chunkManager.isSolid(blockCandidate)){
                closestSolidY = blockCandidate;
                distSolidY = dist;
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
    } else {
        minDist = distSolidY;
        closestSolid = closestSolidY;
    }
    if(distSolidZ<minDist){
        minDist = distSolidZ;
        closestSolid = closestSolidZ;
    }
    if(minDist < maxReach){
        blockTargeted = true;
        targetedBlock = closestSolid;
    } else {
        blockTargeted = false;
    }

}

void World::deleteTarget(){
    std::cout << "Deleting target. BlockTargeted: " << blockTargeted << ", Location: " << targetedBlock.x << "," << targetedBlock.y << "," << targetedBlock.z << std::endl;  
    if(blockTargeted){
        chunkManager.deleteBlock(targetedBlock);
    }

}

void World::update(){
    calculatePlayerTarget();
    std::cout << "BlockTargeted: " << blockTargeted << ", Location: " << targetedBlock.x << "," << targetedBlock.y << "," << targetedBlock.z << std::endl;  

}




