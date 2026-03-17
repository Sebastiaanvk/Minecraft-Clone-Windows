#ifndef CHUNK_HPP
#define CHUNK_HPP


#include <world/block.hpp>
#include <render/renderable.hpp>
#include <util/loc.h>
#include <array>
#include <vector>
#include <set>
#include <memory>
// #include <FastNoiseLite.h>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <world/chunkGeneration.hpp>

// If these constants are changed, make sure to update getChunkID and getLockWithinChunk in the chunkManager class
static constexpr int MAXCHUNKX = 16;
static constexpr int MAXCHUNKY = 256;
static constexpr int MAXCHUNKZ = 16;
static constexpr int CHUNKSIZE = MAXCHUNKX*MAXCHUNKY*MAXCHUNKZ;

static constexpr ChunkID nbDiffs[4] = {{-MAXCHUNKX,0},{0,MAXCHUNKZ},{MAXCHUNKX,0},{0,-MAXCHUNKZ}};
static constexpr ChunkID nbDiffsDiag[8] = {{-MAXCHUNKX,-MAXCHUNKZ},{-MAXCHUNKX,0},{-MAXCHUNKX,MAXCHUNKZ},{0,MAXCHUNKZ},{MAXCHUNKX,MAXCHUNKZ},{MAXCHUNKX,0},{MAXCHUNKX,-MAXCHUNKZ},{0,-MAXCHUNKZ}};


class ChunkManager; // Chunk and ChunkManager include each other.

class Chunk{
public:
    // Chunk(const ChunkID& loc,  ChunkManager& chunkManager, const GenerationPars* generationParsP, FastNoiseLite fastNoiseLite);
    Chunk(const ChunkID& loc,  ChunkManager& chunkManager);
    void generateChunkTerrain();
    void generateTrees(std::array<Chunk*,8> nbChunks);
    void update_mesh(std::array<Chunk*,8> nbChunks);

    std::array<BlockID,CHUNKSIZE> chunk; // This is for faster mesh creation. Kind of ugly though to have no get function.


    //Flags:
    bool isDirty();
    void setDirty();
    bool terrainIsGenerated();
    bool treesAreGenerated();    
    void setGeneratingTreesFlagTrue();
    bool generatingTrees();
    bool getCalculatingMeshFlag();
    void setCalculatingMeshFlagTrue();

    // The LocInt loc parameter of the Chunk methods take the location relative to the Chunk location in the world.
    void setBlockIdNoCheck(const LocInt& loc,BlockID id); // No checks to see if the loc is in range.
    void deleteBlock(LocInt loc);
    BlockID getBlockId(const LocInt& loc) const;
    bool blockIsSolid(const LocInt& loc);
    bool blockIsOpaque(const LocInt& loc) const;
    bool notAir(const LocInt& loc) const;
    bool blockIsUnderwater(const LocInt& loc) const;
    int getHighestYBorder() const;


    std::shared_ptr<RenderableChunkMesh> getMeshPtr();
    
    
private:
    std::atomic<bool> dirtyFlag = true; // I guess dirty is negative of finished creating mesh.
    std::atomic<bool> terrainGeneratedFlag = false;
    std::atomic<bool> treesGeneratedFlag = false;
    std::atomic<bool> generatingTreesFlag = false;
    std::atomic<bool> calculatingMeshFlag = false;

    ChunkManager& chunkManager;
    LocInt chunkLoc;
    ChunkID chunkID;
    std::shared_ptr<RenderableChunkMesh> meshPtr;
    int highestY = 0;
    int highestYBorder = 0;
    BlockID getBlockIdNBs(const LocInt& loc,const std::array<Chunk*,8>& nbs) const;
    uint8_t calcOcclusion(const LocInt& loc,const LocInt& cornerOffset, int orientation, const std::array<Chunk*,8>& nbChunks);
    void setBlockIdNbsIfEmpty(const LocInt& loc,BlockID id,std::array<Chunk*,8> nbs);

    const int cornerOrder[6] = {0,1,3,1,2,3};
    const glm::vec2 uvDiff[4] = {{0,1},{1,1},{1,0},{0,0}};
    void addSolidFaceToMesh(const ChunkMeshElt& face);
    void addCutoutFaceToMesh(const CutoutMeshElt& face);
    void addTranslucentFaceToMesh(const TranslucentMeshElt& face);

};




#endif //CHUNK_HPP