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
#include <external/FastNoiseLite.h>
#include <world/chunkManager.hpp>
#include <world/player.hpp>

class World {
public:
    // World();
    World(unsigned int seed);

    std::queue<std::shared_ptr<RenderableChunkMesh>> toRenderableChunkQueue();
    Player player;
    void deleteTarget();
    void placeBlock();
    void update();

private:
    ChunkManager chunkManager;
    void addChunk(const ChunkID& chunkID);    

    bool blockTargeted;
    LocInt targetedBlock;
    LocInt placementCandidate;
    void calculatePlayerTarget() ;


};

#endif //MODEL_HPP