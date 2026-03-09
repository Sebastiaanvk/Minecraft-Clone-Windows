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
};


struct GameUIData {
    bool paused;
    float frameRate;
};

struct RendererUIData {
    int* maxNewMeshesPerFrameP;
    float* projectionDistanceP;
    float* textureMarginP;
    float* localOutlineOffsetP;
    float* localOutlineWidthP;
    float* hotbarWidthPortionP;
    float* crossHairLengthP;
    float* crossHairWidthP;
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