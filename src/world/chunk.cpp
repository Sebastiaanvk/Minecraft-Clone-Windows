#include <world/chunk.hpp>

Chunk::Chunk(){

}

Chunk::Chunk(const ChunkID& loc){
//    updated = true;
    meshPtr = std::make_shared<RenderableChunkMesh>();
    dirty = true;
    meshPtr->chunkId = loc;
    chunkLoc = {loc.x,0,loc.z};
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
}

// Chunk::Chunk(std::array<BlockID,CHUNKSIZE>& chunkInput, const ChunkID& loc){
// //    updated = true;
//     meshPtr = std::make_shared<RenderableChunkMesh>();
//     meshPtr->chunkId = loc;
//     dirty = true;
//     chunkLoc = {loc.x,0,loc.z};
//     chunk = chunkInput;
// }

Chunk::Chunk(std::vector<std::pair<LocInt,BlockID>> blockSet, const ChunkID& loc){
//    updated = true;
    meshPtr = std::make_shared<RenderableChunkMesh>();
    dirty = true;
    meshPtr->chunkId = loc;
    chunkLoc = {loc.x,0,loc.z};
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
    for(auto& b : blockSet){
        setBlockId(b.first, b.second);
    }
}

BlockID Chunk::getBlockId(const LocInt& loc){
    return chunk[ loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x];
}

void Chunk::setBlockId(const LocInt& loc,BlockID id){
    chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x] = id;
    dirty = true;
}

bool Chunk::blockIsSolid(const LocInt& loc){
    if(loc.x<0||loc.y<0||loc.z<0||loc.x>=MAXCHUNKX || loc.y>=MAXCHUNKY || loc.z>=MAXCHUNKZ){
        return false;
    }
    return BlockRegistry::is_solid( getBlockId(loc) );
}

bool Chunk::isDirty(){
    return dirty;
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
        if(blockIsSolid(loc)){
            for(int i=0; i<6;i++){
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



