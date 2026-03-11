#ifndef CHUNK_GENERATION_HPP
#define CHUNK_GENERATION_HPP

#include <FastNoiseLite.h>
#include <util/loc.h>


namespace ChunkGeneration{

    void init(int seed);

    int getDirtHeight(const Loc2& loc);
    int getWaterLevel();
    int getBedrockHeight();

};


#endif //CHUNK_GENERATION_HPP