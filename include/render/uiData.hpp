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
};


// struct UIData {
//     // GameUIData gameData;
//     CameraUIData cameraData;
//     WorldUIData worldData;
// };



#endif //UIVARS_HPP