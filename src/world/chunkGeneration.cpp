#include <world/chunkGeneration.hpp>

const int expectedDirtHeight = 128;
const int dirtHeightAmplitude = 30;
const int bedrockHeight = 5;
const int waterLevel = 115;
// FastNoiseLite noise;
const int nrNoises = 4;
float amplitudes[nrNoises] = {60,30,15,7.5};
float scales[nrNoises] = {10.0,5.0,2.0,1.0}; 

float poppyChance = 0.03f;
float dandelionChance = 0.03f;
float shortGrassChance = 0.1f;
float treeChance = 0.003f;

float tubeCoralChance = 0.03f;
float brainCoralChance = 0.03f;
float seagrassChance = 0.1f;

uint64_t seed;
FastNoiseLite noises[nrNoises];


// Ok I got these two functions from chatgpt
// Apparently they are very efficient for converting a seed + x + z coordinate into a psuedo random float int [0.0,1.0]
static uint64_t splitmix64(uint64_t z)
{
    z += 0x9e3779b97f4a7c15ULL;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    z = z ^ (z >> 31);
    return z;
}
// Also copied from chatgpt. 
double random2D(int x, int z, uint64_t seed)
{
    uint64_t combined = seed;
    combined ^= (uint64_t)x * 0x9e3779b97f4a7c15ULL;
    combined ^= (uint64_t)z * 0xbf58476d1ce4e5b9ULL;

    uint64_t h = splitmix64(combined);

    return (h >> 11) * (1.0 / (1ULL << 53)); // uniform in [0,1)
}

void ChunkGeneration::init(int inputSeed){
    seed = inputSeed;
    for(int i=0; i<nrNoises; i++){
        noises[i] = FastNoiseLite(seed+i);
    }
}

int ChunkGeneration::getDirtHeight(const Loc2& loc){
    float dirtHeight = expectedDirtHeight;
    for(int i=0;i<nrNoises; i++){
        dirtHeight += amplitudes[i]* noises[i].GetNoise((float)(loc.x)/scales[i],(float)(loc.z)/scales[i]);
    }
    if(dirtHeight>255){
        return 255;
    }
    if(dirtHeight<5){
        return 5;
    }
    
    return dirtHeight;
}

int ChunkGeneration::getWaterLevel(){
    return waterLevel;
}

int ChunkGeneration::getBedrockHeight(){
    return bedrockHeight;
}

BlockID ChunkGeneration::getOptionalPlant(const Loc2& loc){
    float locScore = random2D(loc.x,loc.z, seed);
    if(locScore<poppyChance){
        return BlockID::Poppy;
    }
    locScore -= poppyChance;
    if(locScore<dandelionChance){
        return BlockID::Dandelion;
    }
    locScore -= dandelionChance;
    if(locScore<shortGrassChance){
        return BlockID::Short_Grass;
    }
    return BlockID::Air;
}
BlockID ChunkGeneration::getOptionalUnderwaterPlant(const Loc2& loc){
    float locScore = random2D(loc.x,loc.z, seed);
    if(locScore<tubeCoralChance){
        return BlockID::TubeCoral;
    }
    locScore -= tubeCoralChance;
    if(locScore<brainCoralChance){
        return BlockID::BrainCoral;
    }
    locScore -= brainCoralChance;
    if(locScore<seagrassChance){
        return BlockID::Seagrass;
    }
    return BlockID::Water;
}

bool ChunkGeneration::containsTree(const Loc2& loc){
    float locScore = random2D(loc.x,loc.z, seed);
    return locScore>1.0f-treeChance;
}