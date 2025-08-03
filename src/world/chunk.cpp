#include <world/chunk.hpp>


Chunk::Chunk(const Loc2& loc){
//    updated = true;
    dirty = true;
    chunkLoc = {MAXCHUNKX*loc.x,0,MAXCHUNKY*loc.z};
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
}

Chunk::Chunk(std::array<BlockID,CHUNKSIZE>& chunkInput, const Loc2& loc){
//    updated = true;
    dirty = true;
    chunkLoc = {MAXCHUNKX*loc.x,0,MAXCHUNKY*loc.z};
    chunk = chunkInput;
}

Chunk::Chunk(std::vector<std::pair<LocInt,BlockID>> blockSet, const Loc2& loc){
//    updated = true;
    dirty = true;
    chunkLoc = {MAXCHUNKX*loc.x,0,MAXCHUNKY*loc.z};
    for(int i=0; i<CHUNKSIZE; i++){
        chunk[i] = BlockID::Air;
    }
    for(auto& b : blockSet){
        setBlockId(b.first, b.second);
    }
}


const std::vector<std::vector<LocInt>> blockSides = {
    {{0,0,0},{1,0,0},{1,1,0},{0,1,0}},
    {{0,0,1},{0,1,1},{1,1,0},{1,0,1}},
    {{0,0,0},{0,0,1},{1,0,1},{1,0,0}},
    {{0,1,0},{1,1,0},{1,1,1},{0,1,1}},
    {{0,0,0},{0,1,0},{0,1,1},{0,0,1}},
    {{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
};

BlockID Chunk::getBlockId(const LocInt& loc){
    return chunk[ loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x];
}

void Chunk::setBlockId(const LocInt& loc,BlockID id){
    chunk[loc.y*MAXCHUNKX*MAXCHUNKZ + loc.z*MAXCHUNKX + loc.x] = id;
}

bool Chunk::blockIsSolid(const LocInt& loc){
    if(loc.x<0||loc.y<0||loc.z<0||loc.x>=MAXCHUNKX || loc.y>=MAXCHUNKY || loc.z>=MAXCHUNKZ){
        return false;
    }
    return BlockRegistry::is_solid( getBlockId(loc) );
}

void Chunk::update_mesh(){
    mesh = RenderableChunkMesh();
    mesh.updated =true;
    for(int x=0; x<MAXCHUNKX; x++){ for(int y=0; y<MAXCHUNKY; y++){ for(int z=0; z< MAXCHUNKZ; z++){
        LocInt loc = {x,y,z};
        for(int i=0; i<6;i++){
            LocInt dir = dirs[i];
            if(!blockIsSolid(loc+dir)){
                ChunkMeshElt meshElt;
                meshElt.corner0 = chunkLoc + blockSides[i][0];
                meshElt.corner1 = chunkLoc + blockSides[i][1];
                meshElt.corner2 = chunkLoc + blockSides[i][2];
                meshElt.corner3 = chunkLoc + blockSides[i][3];

                meshElt.blockType = getBlockId(loc+dir);
                meshElt.faceType = static_cast<FaceType>(i);
            }
        }
    }}}
    dirty = false;
}



RenderableChunkMesh Chunk::getMesh(){
    if(dirty){
        update_mesh();
    } else {
        mesh.updated = false;
    }
    return mesh;
}



