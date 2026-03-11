#include <world/chunkGeneration.hpp>

const int expectedDirtHeight = 128;
const int dirtHeightAmplitude = 30;
const int bedrockHeight = 5;
const int waterLevel = 115;
FastNoiseLite noise;

void ChunkGeneration::init(int seed){
    noise = FastNoiseLite(seed);
}

int ChunkGeneration::getDirtHeight(const Loc2& loc){
    return expectedDirtHeight + dirtHeightAmplitude* noise.GetNoise((float)(loc.x),(float)(loc.z));
}

int ChunkGeneration::getWaterLevel(){
    return waterLevel;
}

int ChunkGeneration::getBedrockHeight(){
    return bedrockHeight;
}