#ifndef MODEL_HPP
#define MODEL_HPP

// #include <world/block.hpp>
#include <world/chunk.hpp>
#include <render/renderable.hpp>
#include <util/loc.h>
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <FastNoiseLite.h>
#include <world/chunkManager.hpp>
#include <world/player.hpp>
#include <input/input_handler.hpp>
#include <render/uiData.hpp>

class World {
public:
    // World();
    World(unsigned int seed);

    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue();
    Player player;
    void deleteTarget();
    void placeBlock();
    void update(Input_Handler& input_handler);


    float tickTimeLength = 0.05f;
    WorldUIData getUIData();

private:
    ChunkManager chunkManager;
    void addChunk(const ChunkID& chunkID);    

    bool blockTargeted = false;
    LocInt targetedBlock;
    LocInt placementCandidate;
    void calculatePlayerTarget() ;
    int tick = 0;
    int ticksBetweenBlockManipulation = 7;


};

#endif //MODEL_HPP