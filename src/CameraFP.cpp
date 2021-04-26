#include "cinder/Camera.h"

using namespace ci;
using namespace std;

class CameraFP : public CameraPersp {
protected:
    float mPitch = 0;
    float mYaw = -90;
    float mMouseSensitivity = 0.75;

public:
    void processMouse(float xoffset, float yoffset)
    {
        mYaw += xoffset * mMouseSensitivity;
        mPitch -= yoffset * mMouseSensitivity;

        mPitch = glm::clamp(mPitch, -89.0f, 89.0f);

        vec3 newDir;
        newDir.x = cos(toRadians(mYaw)) * cos(toRadians(mPitch));
        newDir.y = sin(toRadians(mPitch));
        newDir.z = sin(toRadians(mYaw)) * cos(toRadians(mPitch));
        newDir = glm::normalize(newDir);
        lookAt(mEyePoint + newDir);
    }
};