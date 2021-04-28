#include "cinder/Camera.h"

using namespace ci;

enum class MOVEMENT {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UPWARD,
    DOWNWARD,
};

class CameraFP : public CameraPersp {
protected:
    float mPitch = 0;
    float mYaw = -90;
    float mMouseSensitivity = 0.5;
    float mMoveSpeed = 7;
    vec3 mTarget;

public:
    void processMouse(float xoffset, float yoffset);
    void move(MOVEMENT movement, double timeOffset);
};
