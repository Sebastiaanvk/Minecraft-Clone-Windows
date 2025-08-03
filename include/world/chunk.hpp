#ifndef CHUNK_HPP
#define CHUNK_HPP


#include <world/block.hpp>
#include <render/renderable.hpp>
#include <world/blockInfo.hpp>
#include <util/loc.h>
#include <array>
#include <vector>
#include <set>

static const int MAXCHUNKX = 16;
static const int MAXCHUNKY = 256;
static const int MAXCHUNKZ = 16;
static const int CHUNKSIZE = MAXCHUNKX*MAXCHUNKY*MAXCHUNKZ;

class Chunk{
    public:
    Chunk(const Loc2& loc);
    Chunk(std::array<BlockID,CHUNKSIZE>& chunkInput, const Loc2& loc);
    Chunk(std::vector<std::pair<LocInt,BlockID>> blockSet, const Loc2& loc);

    void update_mesh();
    RenderableChunkMesh getMesh();
    BlockID getBlockId(const LocInt& loc);
    void setBlockId(const LocInt& loc,BlockID id);

private:
    std::array<BlockID,CHUNKSIZE> chunk;
    LocInt chunkLoc;
    bool dirty;
//    bool updated;
    RenderableChunkMesh mesh;
    bool blockIsSolid(const LocInt& loc);
};




#endif //CHUNK_HPP