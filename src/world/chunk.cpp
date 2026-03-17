#include <world/chunk.hpp>
#include <world/chunkManager.hpp> // Chunk and ChunkManager include each other.

inline int locToIndex(int x, int y, int z){
    return y*MAXCHUNKX*MAXCHUNKZ + z*MAXCHUNKX + x;
}
inline int locToIndex(const LocInt& locInt) {
    return locInt.y*MAXCHUNKX*MAXCHUNKZ + locInt.z*MAXCHUNKX + locInt.x;
}
Chunk::Chunk(const ChunkID& loc, ChunkManager& chunkManager)
    : chunkManager(chunkManager), chunkLoc({loc.x,0,loc.z}), chunkID(loc)
{
    meshPtr = std::make_shared<RenderableChunkMesh>();
    meshPtr->chunkId = loc;
    // chunkLoc = {loc.x,0,loc.z};
}

void Chunk::generateChunkTerrain(){
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
    for(int x=0; x<MAXCHUNKX; x++){
        for(int z=0; z<MAXCHUNKZ; z++){
            Loc2 realLoc = {x+chunkID.x,z+chunkID.z};
            int dirtHeight = ChunkGeneration::getDirtHeight(realLoc);
            int bedrockHeight = ChunkGeneration::getBedrockHeight();
            int waterLevel = ChunkGeneration::getWaterLevel();

            for(int y=0; y<bedrockHeight; y++){
                setBlockIdNoCheck({x,y,z}, BlockID::Bedrock);
            }
            for(int y=bedrockHeight; y<dirtHeight; y++){
                setBlockIdNoCheck({x,y,z}, BlockID::Dirt);
            }
            for(int y=dirtHeight; y<waterLevel; y++){
                setBlockIdNoCheck({x,y,z}, BlockID::Water);
            }
            if(dirtHeight>=waterLevel){
                setBlockIdNoCheck({x,dirtHeight-1,z},BlockID::Grass_Dirt);
                setBlockIdNoCheck({x,dirtHeight,z},ChunkGeneration::getOptionalPlant(realLoc));
            } else {
                setBlockIdNoCheck({x,dirtHeight,z},ChunkGeneration::getOptionalUnderwaterPlant(realLoc));
            }
        }
    } 
    terrainGeneratedFlag = true;
}

void Chunk::generateTrees(std::array<Chunk*,8> nbChunks){
    for(int x=0; x<MAXCHUNKX; x++){
        for(int z=0; z<MAXCHUNKZ; z++){
            int dirtHeight = ChunkGeneration::getDirtHeight({x+chunkID.x,z+chunkID.z});
            int waterLevel = ChunkGeneration::getWaterLevel();
            if(dirtHeight>=waterLevel){
                if(ChunkGeneration::containsTree({chunkID.x+x,chunkID.z+z})){
                    // for(int i=0; i<8; i++){
                    //     assert(!nbChunks[i]->generatingTrees());
                    //     assert(!nbChunks[i]->getCalculatingMeshFlag());
                    //     assert(nbChunks[i]->terrainIsGenerated());
                    //     assert(nbChunks[i]->terrainIsGenerated());
                    // }
                    for(int y=dirtHeight; y<dirtHeight+6 && y<MAXCHUNKY; y++){
                        setBlockIdNoCheck({x,y,z},BlockID::Oak_Log);
                    }
                    for(int i=0; i<ChunkGeneration::treeLeaves.size(); i++){
                        // std::cout << x+ChunkGeneration::treeLeaves[i].x << "," << dirtHeight+ChunkGeneration::treeLeaves[i].y << "," << z+ChunkGeneration::treeLeaves[i].z << std::endl;
                        setBlockIdNbsIfEmpty({x+ChunkGeneration::treeLeaves[i].x,dirtHeight+ChunkGeneration::treeLeaves[i].y,z+ChunkGeneration::treeLeaves[i].z},BlockID::Oak_Leaves,nbChunks);
                    }
                    
                }
            }
        }
    } 
    treesGeneratedFlag = true;
    generatingTreesFlag = false;
}

BlockID Chunk::getBlockId(const LocInt& loc) const{
    return chunk.at( loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x);
}

void Chunk::setBlockIdNoCheck(const LocInt& loc,BlockID id){
    // assert(loc.x>=0 && loc.x<MAXCHUNKX);
    // assert(loc.y>=0 && loc.y<MAXCHUNKY);
    // assert(loc.z>=0 && loc.z<MAXCHUNKZ);
    chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x] = id;
    dirtyFlag = true;
    if(id!=BlockID::Air){
        highestY = std::max(highestY,loc.y);
        if(loc.x==0 || loc.x==MAXCHUNKX-1 || loc.z==0 || loc.z==MAXCHUNKZ-1){
            highestYBorder = std::max(highestYBorder,loc.y);
        }
    }
}


void Chunk::setBlockIdNbsIfEmpty(const LocInt& loc,BlockID id,std::array<Chunk*,8> nbs){
    // The nbs go in clockwise order, starting in the bottom left (negative x, negative z)
    // 2 3 4
    // 1 x 5
    // 0 7 6
    if(loc.y>=MAXCHUNKY || loc.y<0){
        return;
    }
    int nbIndex;
    int newX = loc.x;
    int newZ = loc.z;

    if(loc.x>=0 && loc.x<MAXCHUNKX){
        if(loc.z>=0 && loc.z<MAXCHUNKZ){
            if(getBlockId(loc)==BlockID::Air)
                setBlockIdNoCheck(loc,id);
            return;
        }
        if(loc.z<0){
            nbIndex=7;
            newZ +=MAXCHUNKZ;
        } else {
            nbIndex=3;
            newZ-=MAXCHUNKZ;
        }
    } else if(loc.x<0){
        newX += MAXCHUNKX;
        if(loc.z<0){
            nbIndex=0;
            newZ +=MAXCHUNKZ;
        } else if(loc.z<MAXCHUNKZ){
            nbIndex=1;
        } else {
            nbIndex=2;
            newZ-=MAXCHUNKZ;
        }
    } else {
        newX -= MAXCHUNKX;
        if(loc.z<0){
            nbIndex=6;
            newZ +=MAXCHUNKZ;
        } else if(loc.z<MAXCHUNKZ){
            nbIndex=5;
        } else {
            nbIndex=4;
            newZ-=MAXCHUNKZ;
        }
    }
    if(nbs[nbIndex]->getBlockId({newX,loc.y,newZ})==BlockID::Air)
        nbs[nbIndex]->setBlockIdNoCheck({newX,loc.y,newZ},id);
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

bool Chunk::blockIsUnderwater(const LocInt& loc) const{
    if(loc.y<0||loc.y>=MAXCHUNKY){
        return false;
    }
    return BlockRegistry::isUnderwater(getBlockId(loc));
}

bool Chunk::isDirty(){
    return dirtyFlag;
}

void Chunk::setDirty(){
    dirtyFlag = true;
}

bool Chunk::terrainIsGenerated(){
    return terrainGeneratedFlag;
}
bool Chunk::treesAreGenerated(){
    return treesGeneratedFlag;
}
bool Chunk::generatingTrees(){
    return generatingTreesFlag;
}
void Chunk::setGeneratingTreesFlagTrue(){
    generatingTreesFlag = true;
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
    setBlockIdNoCheck(loc, BlockID::Air);
}

BlockID Chunk::getBlockIdNBs(const LocInt& loc,const std::array<Chunk*,8>& nbs) const{
    // The nbs go in clockwise order, starting in the bottom left (negative x, negative z)
    // 2 3 4
    // 1 x 5
    // 0 7 6
    if(loc.y>=MAXCHUNKY || loc.y<0){
        return BlockID::Air;
    }
    int nbIndex;
    int newX = loc.x;
    int newZ = loc.z;

    if(loc.x>=0 && loc.x<MAXCHUNKX){
        if(loc.z>=0 && loc.z<MAXCHUNKZ){
            return chunk[locToIndex(loc)];
        }
        if(loc.z<0){
            nbIndex=7;
            newZ +=MAXCHUNKZ;
        } else {
            nbIndex=3;
            newZ-=MAXCHUNKZ;
        }
    } else if(loc.x<0){
        newX += MAXCHUNKX;
        if(loc.z<0){
            nbIndex=0;
            newZ +=MAXCHUNKZ;
        } else if(loc.z<MAXCHUNKZ){
            nbIndex=1;
        } else {
            nbIndex=2;
            newZ-=MAXCHUNKZ;
        }
    } else {
        newX -= MAXCHUNKX;
        if(loc.z<0){
            nbIndex=6;
            newZ +=MAXCHUNKZ;
        } else if(loc.z<MAXCHUNKZ){
            nbIndex=5;
        } else {
            nbIndex=4;
            newZ-=MAXCHUNKZ;
        }
    }
    return nbs[nbIndex]->chunk[locToIndex({newX,loc.y,newZ})];
}

// From the loc.h file:
// inline const LocInt dirs[] = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};


//The corners are counter-clockwise, starting at the left bottom looking inward.
const std::array<std::array<LocInt,4>,6> blockSides = {{
    {{{1,0,0},{1,0,1},{1,1,1},{1,1,0}}},
    {{{0,0,1},{0,0,0},{0,1,0},{0,1,1}}},
    {{{0,1,0},{1,1,0},{1,1,1},{0,1,1}}},
    {{{0,0,1},{1,0,1},{1,0,0},{0,0,0}}},
    {{{1,0,1},{0,0,1},{0,1,1},{1,1,1}}},
    {{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}},
}};

const std::array<LocInt,8> cubeCorners = {{
    {0,0,0},{1,0,0},{1,0,1},{0,0,1},
    {0,1,0},{1,1,0},{1,1,1},{0,1,1}
}};

// const std::array<LocInt,8> cubeCornersDiagonals = {{
//     {-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1},
//     {-1,1,-1},{1,1,-1},{1,1,1},{-1,1,1}
// }};

// const LocInt cubeCornersAdjacent[8][3] = {
//     {{-1,0,0},{0,-1,0},{0,0,-1}},
//     {{1,0,0},{0,-1,0},{0,0,-1}},
//     {{1,0,0},{0,-1,0},{0,0,1}},
//     {{-1,0,0},{0,-1,0},{0,0,1}},
//     {{-1,0,0},{0,1,0},{0,0,-1}},
//     {{1,0,0},{0,1,0},{0,0,-1}},
//     {{1,0,0},{0,1,0},{0,0,1}},
//     {{-1,0,0},{0,1,0},{0,0,1}}
// };

// The ith element of this array are the four corners of the face in direction dirs[i]
// going counter clockwise looking from the inside of the cube.
const std::array<std::array<int,4>,6> blockSidesIndices = {{
    {1,2,6,5},
    {3,0,4,7},
    {4,5,6,7},
    {3,2,1,0},
    {2,3,7,6},
    {0,1,5,4},
}};

// These are very arbitrary. Will need to test.
const std::array<uint8_t,6> occlusions = {
    255,225,150,      // diagonal neighbor not opaque
    225,200,150       // diagonal neighbor opaque.
};
// const std::array<uint8_t,6> occlusions = {
//     255,0,0,      // diagonal neighbor not opaque
//     0,0,0       // diagonal neighbor opaque.
// };

// The cornerOffset give the corner of the cube and the orientation is the orientation of the face: 0:x,1:y,2:z.
uint8_t Chunk::calcOcclusion(const LocInt& loc,const LocInt& cornerOffset, int orientation, const std::array<Chunk*,8>& nbChunks){
    int opaqueNbs = 0;
    LocInt diag = {0,0,0};
    if(orientation!=0){
        if(cornerOffset.x==0){
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x-1,loc.y,loc.z},nbChunks));
            diag.x -=1;
        } else {
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x+1,loc.y,loc.z},nbChunks));
            diag.x +=1;
        }
    }
    if(orientation!=1){
        if(cornerOffset.y==0){
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x,loc.y-1,loc.z},nbChunks));
            diag.y -= 1;
        } else {
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x,loc.y+1,loc.z},nbChunks));
            diag.y += 1;
        }
    }
    if(orientation!=2){
        if(cornerOffset.z==0){
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x,loc.y,loc.z-1},nbChunks));
            diag.z -= 1;
        } else {
            opaqueNbs += BlockRegistry::isOpaque(getBlockIdNBs({loc.x,loc.y,loc.z+1},nbChunks));
            diag.z += 1;
        }
    }
    if(BlockRegistry::isOpaque(getBlockIdNBs(loc+diag,nbChunks))){
        opaqueNbs += 3;
    }
    return occlusions[opaqueNbs];
}

const std::array<std::array<LocInt,4>,4> diagonals = {{
    {{{0,0,0},{1,0,1},{1,1,1},{0,1,0}}},
    {{{1,0,0},{0,0,1},{0,1,1},{1,1,0}}},
    {{{1,0,1},{0,0,0},{0,1,0},{1,1,1}}},
    {{{0,0,1},{1,0,0},{1,1,0},{0,1,1}}}
}};


void Chunk::update_mesh(std::array<Chunk*,8> nbChunks){
    // Ok this function goes against all object oriented principles, but I really needed to optimize as much as possible.
    meshPtr->solidMesh = {};
    meshPtr->cutoutMesh = {};
    meshPtr->translucentMesh = {};


    int maxYToCheck = std::max({highestY,nbChunks[1]->getHighestYBorder(),nbChunks[3]->getHighestYBorder(),nbChunks[5]->getHighestYBorder(),nbChunks[7]->getHighestYBorder()});

    
    // for(int y=-1; y<=maxYToCheck+1; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){
    for(int y=0; y<=maxYToCheck+1; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){

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
                // BlockID nbBlockID = BlockID::Air; 
                // if(nb.x==-1){
                //     // nbOpaque = BlockRegistry::isOpaque(nbChunkNegX.chunk[locToIndex(MAXCHUNKX-1,loc.y,loc.z)]);
                //     nbBlockID = nbChunkNegX->chunk[locToIndex(MAXCHUNKX-1,loc.y,loc.z)];
                // } else if(nb.x==MAXCHUNKX){
                //     // nbOpaque = BlockRegistry::isOpaque(nbChunkPosX.chunk[locToIndex(0,loc.y,loc.z)]);
                //     nbBlockID = nbChunkPosX->chunk[locToIndex(0,loc.y,loc.z)];
                // } else if(nb.z==-1){
                //     // nbOpaque = BlockRegistry::isOpaque(nbChunkNegZ.chunk[locToIndex(loc.x,loc.y,MAXCHUNKZ-1)]);
                //     nbBlockID = nbChunkNegZ->chunk[locToIndex(loc.x,loc.y,MAXCHUNKZ-1)];
                // } else if(nb.z==MAXCHUNKZ){
                //     // nbOpaque = BlockRegistry::isOpaque(nbChunkPosZ.chunk[locToIndex(loc.x,loc.y,0)]);
                //     nbBlockID = nbChunkPosZ->chunk[locToIndex(loc.x,loc.y,0)];
                // } else if(nb.y>=0 && nb.y<MAXCHUNKY){
                //     // nbOpaque = BlockRegistry::isOpaque(chunk[locToIndexLoc(nb)]);
                //     nbBlockID = chunk[locToIndex(nb)];
                // }
                BlockID nbBlockID = getBlockIdNBs(nb,nbChunks);


                if(BlockRegistry::isOpaque(nbBlockID)){
                    ChunkMeshElt meshElt;
                    // We had to change the order, because we are now rendering the face of the neighbor instead of the current cube.
                    meshElt.corners[0] = realLoc + blockSides[i][1];
                    meshElt.occlusion[0] = calcOcclusion(loc,blockSides[i][1],i/2,nbChunks) ;

                    meshElt.corners[1] = realLoc + blockSides[i][0];
                    meshElt.occlusion[1] = calcOcclusion(loc,blockSides[i][0],i/2,nbChunks) ;

                    meshElt.corners[2] = realLoc + blockSides[i][3];
                    meshElt.occlusion[2] = calcOcclusion(loc,blockSides[i][3],i/2,nbChunks) ;

                    meshElt.corners[3] = realLoc + blockSides[i][2];
                    meshElt.occlusion[3] = calcOcclusion(loc,blockSides[i][2],i/2,nbChunks) ;

                    meshElt.blockType = nbBlockID;
                    // meshElt.blockType = blockId;
                    meshElt.faceType = faceTypeArrIngoing[i];
                    
                    //Fix this for general cases!!!!
                    // Also the color was kind of arbitrary
                    if(meshElt.faceType==FaceType::Top && meshElt.blockType==BlockID::Grass_Dirt){
                        meshElt.tint[0] = grassDirtTint[0],meshElt.tint[1] = grassDirtTint[1],meshElt.tint[2] = grassDirtTint[2];
                        // meshElt.tint[0] = 180,meshElt.tint[1] = 255-15,meshElt.tint[2] = 100;
                    } else {
                        meshElt.tint[0] = 255,meshElt.tint[1] = 255,meshElt.tint[2] = 255;
                    }
                    addSolidFaceToMesh(meshElt);
                // } else if(nbBlockID!=BlockID::Air && !BlockRegistry::isCross(nbBlockID) && !BlockRegistry::isTranslucent(nbBlockID)){ // I think I need to revise the blockRegistry flags, but this should work for now.
                } else if(nbBlockID==BlockID::Oak_Leaves){ 
                    CutoutMeshElt meshElt;
                    meshElt.corners[0] = locIntToLocFloat(realLoc + blockSides[i][1]);
                    meshElt.corners[1] = locIntToLocFloat(realLoc + blockSides[i][0]);
                    meshElt.corners[2] = locIntToLocFloat(realLoc + blockSides[i][3]);
                    meshElt.corners[3] = locIntToLocFloat(realLoc + blockSides[i][2]);
                    meshElt.blockType = nbBlockID;
                    meshElt.tint[0] = leavesTint[0],meshElt.tint[1] = leavesTint[1],meshElt.tint[2] = leavesTint[2];
                    addCutoutFaceToMesh(meshElt);
                } else if( BlockRegistry::isUnderwater(nbBlockID) && (y==MAXCHUNKY || !BlockRegistry::isUnderwater(chunk[locToIndex(x,y,z)]))){ 
                    TranslucentMeshElt meshElt;
                    meshElt.corners[0] = locIntToLocFloat(realLoc + blockSides[i][1]);
                    meshElt.corners[1] = locIntToLocFloat(realLoc + blockSides[i][0]);
                    meshElt.corners[2] = locIntToLocFloat(realLoc + blockSides[i][3]);
                    meshElt.corners[3] = locIntToLocFloat(realLoc + blockSides[i][2]);
                    meshElt.blockType = BlockID::Water;
                    meshElt.faceType = faceTypeArrIngoing[i];
                    meshElt.tint[0] = waterTint[0],meshElt.tint[1] = waterTint[1],meshElt.tint[2] = waterTint[2];
                    addTranslucentFaceToMesh(meshElt);

                    TranslucentMeshElt meshElt2; // Water renders the faces both ways
                    meshElt2.corners[1] = locIntToLocFloat(realLoc + blockSides[i][1]);
                    meshElt2.corners[0] = locIntToLocFloat(realLoc + blockSides[i][0]);
                    meshElt2.corners[3] = locIntToLocFloat(realLoc + blockSides[i][3]);
                    meshElt2.corners[2] = locIntToLocFloat(realLoc + blockSides[i][2]);
                    meshElt2.blockType = BlockID::Water;
                    meshElt2.faceType = faceTypeArrIngoing[i];
                    meshElt2.tint[0] = waterTint[0],meshElt2.tint[1] = waterTint[1],meshElt2.tint[2] = waterTint[2];
                    addTranslucentFaceToMesh(meshElt2);
                }

            }
        }
    }}}

    for(int y=0; y<=maxYToCheck; y++){ for(int z=0; z<MAXCHUNKZ; z++){ for(int x=0; x< MAXCHUNKX; x++){
        LocInt loc = {x,y,z};
        LocInt realLoc = chunkLoc+loc;
        if(BlockRegistry::isCross(chunk[locToIndex(x,y,z)])){
            for(int i=0; i<4; i++){
                CutoutMeshElt cutoutMeshElt;
                cutoutMeshElt.corners[0] = locIntToLocFloat( realLoc + diagonals[i][0]);
                cutoutMeshElt.corners[1] = locIntToLocFloat( realLoc + diagonals[i][1]);
                cutoutMeshElt.corners[2] = locIntToLocFloat( realLoc + diagonals[i][2]);
                cutoutMeshElt.corners[3] = locIntToLocFloat( realLoc + diagonals[i][3]);
                cutoutMeshElt.blockType = chunk[locToIndex(x,y,z)];
                if(chunk[locToIndex(x,y,z)]==BlockID::Short_Grass){
                    cutoutMeshElt.tint[0] = grassTint[0],cutoutMeshElt.tint[1] =grassTint[1],cutoutMeshElt.tint[2] = grassTint[2];
                }
                else {
                    cutoutMeshElt.tint[0] = 255,cutoutMeshElt.tint[1] = 255,cutoutMeshElt.tint[2] = 255;
                }
                addCutoutFaceToMesh(cutoutMeshElt);
            }
        }
    }}}

    dirtyFlag = false;
    calculatingMeshFlag = false;
    meshPtr->updated = true;
}


void Chunk::addSolidFaceToMesh(const ChunkMeshElt& face){
    // Triangles counter-clockwise
    bool switchDiagonal = face.occlusion[1]+face.occlusion[3]>face.occlusion[0]+face.occlusion[4];
    for(int i=0;i<6;i++){
        SolidVBOElt vboElt;
        int corner;
        if(switchDiagonal)
            corner = cornerOrder[i];
        else
            corner = cornerOrder2[i];
        
        vboElt.pos[0] = face.corners[corner].x;
        vboElt.pos[1] = face.corners[corner].y;
        vboElt.pos[2] = -face.corners[corner].z; // Mirror the z!
        vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,face.faceType));
        vboElt.tint[0] = face.tint[0];
        vboElt.tint[1] = face.tint[1];
        vboElt.tint[2] = face.tint[2];
        vboElt.tint[3] = 255;
        vboElt.occlusion = face.occlusion[corner];
        meshPtr->solidMesh.push_back(vboElt);
    }
}
void Chunk::addCutoutFaceToMesh(const CutoutMeshElt& face){
    for(int i=0;i<6;i++){
        CutoutVBOElt vboElt;
        int corner = cornerOrder[i];
        
        vboElt.pos[0] = face.corners[corner].x;
        vboElt.pos[1] = face.corners[corner].y;
        vboElt.pos[2] = -face.corners[corner].z; // mirror the z!
        vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,FaceType::Side));
        vboElt.tint[0] = face.tint[0];
        vboElt.tint[1] = face.tint[1];
        vboElt.tint[2] = face.tint[2];
        vboElt.tint[3] = 255;
        meshPtr->cutoutMesh.push_back(vboElt);
    }
}
void Chunk::addTranslucentFaceToMesh(const TranslucentMeshElt& face){
        for(int i=0;i<6;i++){
            TranslucentVBOElt vboElt;
            int corner = cornerOrder[i];
            
            vboElt.pos[0] = face.corners[corner].x;
            vboElt.pos[1] = face.corners[corner].y;
            vboElt.pos[2] = -face.corners[corner].z; // Mirror the z!
            vboElt.uvl = glm::vec3(uvDiff[corner],BlockRegistry::getTextureIndex(face.blockType,face.faceType));
            vboElt.tint[0] = face.tint[0];
            vboElt.tint[1] = face.tint[1];
            vboElt.tint[2] = face.tint[2];
            vboElt.tint[3] = 255;
            meshPtr->translucentMesh.push_back(vboElt);
        }
}

std::shared_ptr<RenderableChunkMesh> Chunk::getMeshPtr(){
    assert(!dirtyFlag);
    return meshPtr;
}



