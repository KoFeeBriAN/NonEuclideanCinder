#include "cinder/Camera.h"

using namespace ci;
using namespace std;

enum class MOVEMENT { FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN };

class CameraFP : public CameraPersp {
protected:
    float mPitch = 0;

    float mYaw = -90;
    float mMouseSensitivity = 0.5;
    float mMoveSpeed = 20;
    vec3 mTarget;

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

        mTarget = mEyePoint + newDir;
        lookAt(mTarget);
    }

    void move(MOVEMENT movement, double timeOffset)
    {
        float speed = mMoveSpeed * timeOffset;

        if (movement == MOVEMENT::FORWARD)
            mEyePoint -= mW * speed;
        if (movement == MOVEMENT::BACKWARD)
            mEyePoint += mW * speed;
        if (movement == MOVEMENT::LEFT)
            mEyePoint -= mU * speed;
        if (movement == MOVEMENT::RIGHT)
            mEyePoint += mU * speed;

        lookAt(mEyePoint + mViewDirection);
    }
};