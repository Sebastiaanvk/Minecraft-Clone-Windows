#include <world/chunk.hpp>
#include <world/chunkManager.hpp>




Chunk::Chunk(FastNoiseLite& noise, const ChunkID& loc, GenerationPars genPars,ChunkManager& chunkManager)
    : chunkManager(chunkManager), chunkLoc({loc.x,0,loc.z})
{
    meshPtr = std::make_shared<RenderableChunkMesh>();
    meshPtr->chunkId = loc;
    // chunkLoc = {loc.x,0,loc.z};
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
    for(int x=0; x<MAXCHUNKX; x++){
        for(int z=0; z<MAXCHUNKZ; z++){
            int dirtHeight = genPars.expected_dirt_height+ genPars.dirt_height_amplitude* noise.GetNoise((float)(x+loc.x),(float)(z+loc.z));
            for(int y=0; y<genPars.bedrock_height; y++){
                setBlockId({x,y,z}, BlockID::Bedrock);
            }
            for(int y=genPars.bedrock_height; y<dirtHeight-1; y++){
                setBlockId({x,y,z}, BlockID::Dirt);
            }
            setBlockId({x,dirtHeight-1,z},BlockID::Grass_Dirt);
        }
    } 

}

BlockID Chunk::getBlockId(const LocInt& loc) const{
    return chunk.at( loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x);
}

void Chunk::setBlockId(const LocInt& loc,BlockID id){
    chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x] = id;
    dirty = true;
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

bool Chunk::isDirty(){
    return dirty;
}

void Chunk::setDirty(){
    dirty = true;
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

void Chunk::update_mesh(){
    meshPtr->updated =true;
    meshPtr->mesh = {};
    for(int x=0; x<MAXCHUNKX; x++){ for(int y=0; y<MAXCHUNKY; y++){ for(int z=0; z< MAXCHUNKZ; z++){
        LocInt loc = {x,y,z};
        // Change later for see through shizzle.
        // std::cout << loc << std::endl;
        // std::cout << loc.x << "," << loc.y << "," << loc.z << std::endl;
        if(blockIsSolid(loc)){
            for(int i=0; i<6;i++){
                // std::cout << i << std::endl;
                LocInt dir = dirs[i];
                if(!blockIsSolid(loc+dir)){
                    ChunkMeshElt meshElt;
                    meshElt.corners[0] = chunkLoc + loc + blockSides[i][0];
                    meshElt.corners[1] = chunkLoc + loc + blockSides[i][1];
                    meshElt.corners[2] = chunkLoc + loc + blockSides[i][2];
                    meshElt.corners[3] = chunkLoc + loc + blockSides[i][3];

                    meshElt.blockType = getBlockId(loc);
                    meshElt.faceType = faceTypeArr[i];
                    
                    //Fix this for general cases!!!!
                    // Also the color was kind of arbitrary
                    if(meshElt.faceType==FaceType::Top && meshElt.blockType==BlockID::Grass_Dirt){
                        meshElt.tint[0] = 190,meshElt.tint[1] = 255,meshElt.tint[2] = 120;
                    } else {
                        meshElt.tint[0] = 255,meshElt.tint[1] = 255,meshElt.tint[2] = 255;
                    }

                    meshPtr->mesh.push_back(meshElt);
                }
            }
        }
    }}}
    dirty = false;
}



std::shared_ptr<RenderableChunkMesh> Chunk::getMeshPtr(){
    if(dirty){
        update_mesh();
    } 
    return meshPtr;
}



