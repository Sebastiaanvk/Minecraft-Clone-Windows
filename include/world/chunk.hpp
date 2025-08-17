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
#include <iostream>

// If these constants are changed, make sure to update getChunkID and getLockWithinChunk in the chunkManager class
static constexpr int MAXCHUNKX = 16;
static constexpr int MAXCHUNKY = 256;
static constexpr int MAXCHUNKZ = 16;
static constexpr int CHUNKSIZE = MAXCHUNKX*MAXCHUNKY*MAXCHUNKZ;

class ChunkManager;

class Chunk{
    public:

    struct GenerationPars{
        int expected_dirt_height;
        int dirt_height_amplitude;
        int bedrock_height;
    };
    // Chunk();
    // Chunk(const ChunkID& loc);
    // Chunk(std::array<BlockID,CHUNKSIZE>& chunkInput, const ChunkID& loc);
    // Chunk(std::vector<std::pair<LocInt,BlockID>> blockSet, const ChunkID& loc);
    Chunk(FastNoiseLite& noise,const ChunkID& loc, GenerationPars genPars, ChunkManager& chunkManager);

    void update_mesh();
    std::shared_ptr<RenderableChunkMesh> getMeshPtr();
    BlockID getBlockId(const LocInt& loc) const;
    bool blockIsSolid(const LocInt& loc);
    void setBlockId(const LocInt& loc,BlockID id);
    bool isDirty();


private:
    ChunkManager& chunkManager;
    std::array<BlockID,CHUNKSIZE> chunk;
    LocInt chunkLoc;
    bool dirty;
    std::shared_ptr<RenderableChunkMesh> meshPtr;
};




#endif //CHUNK_HPP