#ifndef CHUNK_GENERATION_HPP
#define CHUNK_GENERATION_HPP

#include <FastNoiseLite.h>
#include <cstdint>
#include <util/loc.h>
#include <blockRegistry.hpp>


namespace ChunkGeneration{

    void init(int inputSeed);

    int getDirtHeight(const Loc2& loc);
    int getWaterLevel();
    int getBedrockHeight();
    BlockID getOptionalPlant(const Loc2& loc);
    BlockID getOptionalUnderwaterPlant(const Loc2& loc);
    bool containsTree(const Loc2& loc);

};


#endif //CHUNK_GENERATION_HPP