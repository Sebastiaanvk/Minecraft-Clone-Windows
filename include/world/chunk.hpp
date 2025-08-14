#ifndef CHUNK_HPP
#define CHUNK_HPP


#include <world/block.hpp>
#include <render/renderable.hpp>
#include <util/loc.h>
#include <array>
#include <vector>
#include <set>
#include <memory>
#include <external/FastNoiseLite.h>

static const int MAXCHUNKX = 16;
static const int MAXCHUNKY = 256;
static const int MAXCHUNKZ = 16;
static const int CHUNKSIZE = MAXCHUNKX*MAXCHUNKY*MAXCHUNKZ;

class Chunk{
    public:

    struct GenerationPars{
        int expected_dirt_height;
        int dirt_height_amplitude;
        int bedrock_height;
    };
    Chunk();
    Chunk(const ChunkID& loc);
    // Chunk(std::array<BlockID,CHUNKSIZE>& chunkInput, const ChunkID& loc);
    Chunk(std::vector<std::pair<LocInt,BlockID>> blockSet, const ChunkID& loc);
    Chunk(FastNoiseLite& noise,const ChunkID& loc, GenerationPars genPars);

    void update_mesh();
    std::shared_ptr<RenderableChunkMesh> getMeshPtr();
    BlockID getBlockId(const LocInt& loc);
    void setBlockId(const LocInt& loc,BlockID id);
    bool isDirty();


private:
    std::array<BlockID,CHUNKSIZE> chunk;
    LocInt chunkLoc;
    bool dirty;
//    bool updated;
    std::shared_ptr<RenderableChunkMesh> meshPtr;
    bool blockIsSolid(const LocInt& loc);
};




#endif //CHUNK_HPP