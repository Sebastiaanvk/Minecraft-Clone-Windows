#ifndef UIVARS_HPP
#define UIVARS_HPP

#include "util/loc.h"

struct CameraUIData {
    float* fovP;
};

struct PlayerUIData {
    LocFloat pos;
    LocFloat forwardDirection;
    float* playerSpeedP;
    float* rotationSensitivityP;
    int* hotbarSelectionP;
};

struct WorldUIData {
    PlayerUIData playerData;
    bool blockTargeted;
    LocInt targetedBlock;
    int tick;
    int* ticksBetweenBlockManipulationP;
    float* tickTimeLengthP;
    bool* objectCollisionP;
};


struct GameUIData {
    bool paused;
    float frameRate;
};

struct RendererUIData {
    int* maxNewMeshesPerFrameP;
    float* projectionFarDistanceP;
    float* projectionNearDistanceP;
    float* textureMarginP;
    float* localOutlineOffsetP;
    float* localOutlineWidthP;
    float* hotbarWidthPortionP;
    float* crossHairLengthP;
    float* crossHairWidthP;
    int* maxTerrainRenders;
    int* maxWaterRenders;
    int* maxVegetationRenders; 
    bool* ambientOcclusion;
    bool* gammaCorrection;
};


// struct InputData {
//     float scrollDiff;
// };


// struct UIData {
//     // GameUIData gameData;
//     CameraUIData cameraData;
//     WorldUIData worldData;
// };



#endif //UIVARS_HPP