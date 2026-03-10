#include <world/chunk.hpp>
#include <world/chunkManager.hpp> // Chunk and ChunkManager include each other.

Chunk::Chunk(const ChunkID& loc, ChunkManager& chunkManager, const GenerationPars* generationParsP,  FastNoiseLite fastNoiseLite)
    : chunkManager(chunkManager), chunkLoc({loc.x,0,loc.z}), genParsP(generationParsP), noise(fastNoiseLite)
{
    meshPtr = std::make_shared<RenderableChunkMesh>();
    meshPtr->chunkId = loc;
    // chunkLoc = {loc.x,0,loc.z};
}

void Chunk::generateChunk(){
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
    for(int x=0; x<MAXCHUNKX; x++){
        for(int z=0; z<MAXCHUNKZ; z++){
            int dirtHeight = genParsP->expected_dirt_height+ genParsP->dirt_height_amplitude* noise.GetNoise((float)(x+chunkLoc.x),(float)(z+chunkLoc.z));
            for(int y=0; y<genParsP->bedrock_height; y++){
                setBlockId({x,y,z}, BlockID::Bedrock);
            }
            for(int y=genParsP->bedrock_height; y<dirtHeight-1; y++){
                setBlockId({x,y,z}, BlockID::Dirt);
            }
            setBlockId({x,dirtHeight-1,z},BlockID::Grass_Dirt);
        }
    } 
    chunkGeneratedFlag = true;
}

BlockID Chunk::getBlockId(const LocInt& loc) const{
    return chunk.at( loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x);
}

void Chunk::setBlockId(const LocInt& loc,BlockID id){
    chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x] = id;
    dirtyFlag = true;
    if(id!=BlockID::Air){
        highestY = std::max(highestY,loc.y);
        if(loc.x==0 || loc.x==MAXCHUNKX-1 || loc.z==0 || loc.z==MAXCHUNKZ-1){
            highestYBorder = std::max(highestYBorder,loc.y);
        }
    }
}

bool Chunk::blockIsSolid(const LocInt& loc){
    // std::cout << "Inside blockIsSolid" << std::endl;
    if(loc.y<0||loc.y>=MAXCHUNKY){
        return false;
    }
    if(loc.x<0||loc.z<0||loc.x>=MAXCHUNKX || loc.z>=MAXCHUNKZ ){
        LocInt newloc = loc+LocInt{chunkLoc.x,0,chunkLoc.z};
        // std::cout << newloc.x << ", " << newloc.y << ", " << newloc.z << std::endl;
        // return false;
        return chunkManager.isSolid(newloc);
    }
    return BlockRegistry::is_solid( getBlockId(loc) );
}

bool Chunk::blockIsOpaque(const LocInt& loc) const{
    // std::cout << "Inside blockIsSolid" << std::endl;
    if(loc.y<0||loc.y>=MAXCHUNKY){
        return false;
    }
    if(loc.x<0||loc.z<0||loc.x>=MAXCHUNKX || loc.z>=MAXCHUNKZ ){
        LocInt newloc = loc+LocInt{chunkLoc.x,0,chunkLoc.z};
        return chunkManager.isOpaque(newloc);
    }
    return BlockRegistry::isOpaque( getBlockId(loc) );
}

bool Chunk::notAir(const LocInt& loc) const{
    if(loc.y<0||loc.y>=MAXCHUNKY){
        return false;
    }
    if(loc.x<0||loc.z<0||loc.x>=MAXCHUNKX || loc.z>=MAXCHUNKZ ){
        LocInt newloc = loc+LocInt{chunkLoc.x,0,chunkLoc.z};
        return chunkManager.notAir(newloc);
    }
    return getBlockId(loc)!=BlockID::Air;
}

bool Chunk::isDirty(){
    return dirtyFlag;
}

void Chunk::setDirty(){
    dirtyFlag = true;
}


bool Chunk::getCalculatingMeshFlag(){
    return calculatingMeshFlag;
}

void Chunk::setCalculatingMeshFlagTrue(){
    calculatingMeshFlag = true;
}


int Chunk::getHighestYBorder() const{
    return highestYBorder;
}



void Chunk::deleteBlock(LocInt loc){
    setBlockId(loc, BlockID::Air);
}

//The corners are counter-clockwise, starting at the left bottom
const std::vector<std::vector<LocInt>> blockSides = {
    {{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
    {{0,0,1},{0,0,0},{0,1,0},{0,1,1}},
    {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},
    {{0,0,1},{1,0,1},{1,0,0},{0,0,0}},
    {{1,0,1},{0,0,1},{0,1,1},{1,1,1}},
    {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
};

const std::vector<LocInt> diagonal1 = {
    {0,0,0},{1,0,1},{1,1,1},{0,1,0}
};

const std::vector<LocInt> diagonal2 = {
    {1,0,0},{0,0,1},{0,1,1},{1,1,0}
};

void Chunk::update_mesh(Chunk* nbChunkNegX,Chunk* nbChunkPosX,Chunk* nbChunkNegZ, Chunk* nbChunkPosZ){
    // Ok this function goes against all object oriented principles, but I really needed to optimize as much as possible.
    // Manually check that the array indices are in range!
    meshPtr->solidMesh = {};
    meshPtr->cutoutMesh = {};
    // meshPtr->mesh.reserve(MAXCHUNKX * MAXCHUNKY * MAXCHUNKZ * 6 / 4); // Arbitrary size, not sure if it matters for speed.

    // const Chunk& nbChunkNegX = chunkManager.getChunkPointer({chunkLoc.x-MAXCHUNKX,chunkLoc.z});
    // const Chunk&  nbChunkPosX= chunkManager.getChunkPointer({chunkLoc.x+MAXCHUNKX,chunkLoc.z});
    // const Chunk& nbChunkNegZ = chunkManager.getChunkPointer({chunkLoc.x,chunkLoc.z-MAXCHUNKZ});
    // const Chunk& nbChunkPosZ = chunkManager.getChunkPointer({chunkLoc.x,chunkLoc.z+MAXCHUNKZ});

    int maxYToCheck = std::max({highestY,nbChunkNegX->getHighestYBorder(),nbChunkPosX->getHighestYBorder(),nbChunkNegZ->getHighestYBorder(),nbChunkPosZ->getHighestYBorder()});

    auto locToIndex = [](int x, int y, int z) {
        return y*MAXCHUNKX*MAXCHUNKZ + z*MAXCHUNKX + x;
    };
    auto locToIndexLoc = [](const LocInt& locInt) {
        return locInt.y*MAXCHUNKX*MAXCHUNKZ + locInt.z*MAXCHUNKX + locInt.x;
    };
    
    // for(int y=-1; y<=maxYToCheck+1; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){
    for(int y=0; y<=maxYToCheck+1; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){
        
        // BlockID blockId = chunk[locToIndex(x,y,z)];
        // Change later for see through shizzle.

        // if(y==-1 || y==MAXCHUNKY ||  !BlockRegistry::isOpaque(chunk[locToIndex(x,y,z)])){
        if(y==MAXCHUNKY ||  !BlockRegistry::isOpaque(chunk[locToIndex(x,y,z)])){
            LocInt loc = {x,y,z};
            LocInt realLoc = chunkLoc+loc;
            for(int i=0; i<6;i++){
                LocInt nb = loc+dirs[i];
                if(nb.y<0||nb.y>=MAXCHUNKY){
                    continue;
                }
                // bool nbOpaque = false;
                BlockID nbBlockID = BlockID::Air; 
                if(nb.x==-1){
                    // nbOpaque = BlockRegistry::isOpaque(nbChunkNegX.chunk[locToIndex(MAXCHUNKX-1,loc.y,loc.z)]);
                    nbBlockID = nbChunkNegX->chunk[locToIndex(MAXCHUNKX-1,loc.y,loc.z)];
                } else if(nb.x==MAXCHUNKX){
                    // nbOpaque = BlockRegistry::isOpaque(nbChunkPosX.chunk[locToIndex(0,loc.y,loc.z)]);
                    nbBlockID = nbChunkPosX->chunk[locToIndex(0,loc.y,loc.z)];
                } else if(nb.z==-1){
                    // nbOpaque = BlockRegistry::isOpaque(nbChunkNegZ.chunk[locToIndex(loc.x,loc.y,MAXCHUNKZ-1)]);
                    nbBlockID = nbChunkNegZ->chunk[locToIndex(loc.x,loc.y,MAXCHUNKZ-1)];
                } else if(nb.z==MAXCHUNKZ){
                    // nbOpaque = BlockRegistry::isOpaque(nbChunkPosZ.chunk[locToIndex(loc.x,loc.y,0)]);
                    nbBlockID = nbChunkPosZ->chunk[locToIndex(loc.x,loc.y,0)];
                } else if(nb.y>=0 && nb.y<MAXCHUNKY){
                    // nbOpaque = BlockRegistry::isOpaque(chunk[locToIndexLoc(nb)]);
                    nbBlockID = chunk[locToIndexLoc(nb)];
                }

                if(BlockRegistry::isOpaque(nbBlockID)){
                    ChunkMeshElt meshElt;
                    // We had to change the order, because we are now rendering the face of the neighbor instead of the current cube.
                    meshElt.corners[0] = realLoc + blockSides[i][1];
                    meshElt.corners[1] = realLoc + blockSides[i][0];
                    meshElt.corners[2] = realLoc + blockSides[i][3];
                    meshElt.corners[3] = realLoc + blockSides[i][2];

                    // meshElt.blockType = chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x];
                    meshElt.blockType = nbBlockID;
                    // meshElt.blockType = blockId;
                    meshElt.faceType = faceTypeArrIngoing[i];
                    
                    //Fix this for general cases!!!!
                    // Also the color was kind of arbitrary
                    if(meshElt.faceType==FaceType::Top && meshElt.blockType==BlockID::Grass_Dirt){
                        meshElt.tint[0] = 190,meshElt.tint[1] = 255,meshElt.tint[2] = 120;
                    } else {
                        meshElt.tint[0] = 255,meshElt.tint[1] = 255,meshElt.tint[2] = 255;
                    }
                    meshPtr->solidMesh.push_back(meshElt);
                }
            }
        }
    }}}

    for(int y=0; y<=maxYToCheck; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){
        LocInt loc = {x,y,z};
        LocInt realLoc = chunkLoc+loc;
        if(BlockRegistry::isFlower(chunk[locToIndex(x,y,z)])){
            // std::cout << "Flower found in update mesh!" << std::endl;
            CutoutMeshElt cutoutMeshElt1;
            CutoutMeshElt cutoutMeshElt2;
            cutoutMeshElt1.corners[0] = locIntToLocFloat( realLoc + diagonal1[0]);
            cutoutMeshElt1.corners[1] = locIntToLocFloat( realLoc + diagonal1[1]);
            cutoutMeshElt1.corners[2] = locIntToLocFloat( realLoc + diagonal1[2]);
            cutoutMeshElt1.corners[3] = locIntToLocFloat( realLoc + diagonal1[3]);
            cutoutMeshElt2.corners[0] = locIntToLocFloat( realLoc + diagonal2[0]);
            cutoutMeshElt2.corners[1] = locIntToLocFloat( realLoc + diagonal2[1]);
            cutoutMeshElt2.corners[2] = locIntToLocFloat( realLoc + diagonal2[2]);
            cutoutMeshElt2.corners[3] = locIntToLocFloat( realLoc + diagonal2[3]);
            cutoutMeshElt1.blockType = chunk[locToIndex(x,y,z)];
            cutoutMeshElt2.blockType = chunk[locToIndex(x,y,z)];
            meshPtr->cutoutMesh.push_back(cutoutMeshElt1);
            meshPtr->cutoutMesh.push_back(cutoutMeshElt2);
        }
    }}}

    dirtyFlag = false;
    calculatingMeshFlag = false;
    meshPtr->updated = true;
}


// Before optimizations:
// void Chunk::update_mesh(){
//     meshPtr->updated =true;
//     meshPtr->mesh = {};
//     for(int x=0; x<MAXCHUNKX; x++){ for(int y=0; y<MAXCHUNKY; y++){ for(int z=0; z< MAXCHUNKZ; z++){
//         LocInt loc = {x,y,z};
//         // Change later for see through shizzle.
//         // std::cout << loc << std::endl;
//         // std::cout << loc.x << "," << loc.y << "," << loc.z << std::endl;
//         if(blockIsOpaque(loc)){
//             for(int i=0; i<6;i++){
//                 // std::cout << i << std::endl;
//                 LocInt dir = dirs[i];
//                 if(!blockIsOpaque(loc+dir)){
//                     ChunkMeshElt meshElt;
//                     meshElt.corners[0] = chunkLoc + loc + blockSides[i][0];
//                     meshElt.corners[1] = chunkLoc + loc + blockSides[i][1];
//                     meshElt.corners[2] = chunkLoc + loc + blockSides[i][2];
//                     meshElt.corners[3] = chunkLoc + loc + blockSides[i][3];

//                     meshElt.blockType = getBlockId(loc);
//                     meshElt.faceType = faceTypeArr[i];
                    
//                     //Fix this for general cases!!!!
//                     // Also the color was kind of arbitrary
//                     if(meshElt.faceType==FaceType::Top && meshElt.blockType==BlockID::Grass_Dirt){
//                         meshElt.tint[0] = 190,meshElt.tint[1] = 255,meshElt.tint[2] = 120;
//                     } else {
//                         meshElt.tint[0] = 255,meshElt.tint[1] = 255,meshElt.tint[2] = 255;
//                     }

//                     meshPtr->mesh.push_back(meshElt);
//                 }
//             }
//         }
//     }}}
//     dirty = false;
// }


std::shared_ptr<RenderableChunkMesh> Chunk::getMeshPtr(){
    assert(!dirtyFlag);
    return meshPtr;
}

bool Chunk::isGenerated(){
    return chunkGeneratedFlag;
}


