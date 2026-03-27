#ifndef CHUNK_GENERATION_HPP
#define CHUNK_GENERATION_HPP

#include <FastNoiseLite.h>
#include <cstdint>
#include <util/loc.h>
#include <blockRegistry.hpp>


namespace ChunkGeneration{

    inline const std::vector<LocInt> treeLeaves = {
                            {0,6,-1},
                  {-1,6,0}, {0,6,0},{1,6,0}, 
                            {0,6,1},

                  {-1,5,-1},{0,5,-1},{1,5,-1},
                  {-1,5,0}          ,{1,5,0}, 
                  {-1,5,1}, {0,5,1}, {1,5,1}, 

        {-2,4,-2},{-1,4,-2},{0,4,-2},{1,4,-2},{2,4,-2},
        {-2,4,-1},{-1,4,-1},{0,4,-1},{1,4,-1},{2,4,-1},
        {-2,4,0}, {-1,4,0}          ,{1,4,0}, {2,4,0},
        {-2,4,1}, {-1,4,1}, {0,4,1}, {1,4,1}, {2,4,1},
        {-2,4,2}, {-1,4,2}, {0,4,2}, {1,4,2}, {2,4,2},

        {-2,3,-2},{-1,3,-2},{0,3,-2},{1,3,-2},{2,3,-2},
        {-2,3,-1},{-1,3,-1},{0,3,-1},{1,3,-1},{2,3,-1},
        {-2,3,0}, {-1,3,0}          ,{1,3,0}, {2,3,0},
        {-2,3,1}, {-1,3,1}, {0,3,1}, {1,3,1}, {2,3,1},
        {-2,3,2}, {-1,3,2}, {0,3,2}, {1,3,2}, {2,3,2}
    };

    void init(int inputSeed);

    int getDirtHeight(const Loc2& loc);
    int getWaterLevel();
    int getBedrockHeight();
    BlockID getOptionalPlant(const Loc2& loc);
    BlockID getOptionalUnderwaterPlant(const Loc2& loc);
    bool containsTree(const Loc2& loc);

};


#endif //CHUNK_GENERATION_HPP