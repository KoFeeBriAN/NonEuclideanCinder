#include "cinder/Camera.h"

using namespace ci;
using namespace std;

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

        switch (movement) {
        case MOVEMENT::FORWARD:
            mEyePoint -= mW * speed;
            break;
        case MOVEMENT::BACKWARD:
            mEyePoint += mW * speed;
            break;
        case MOVEMENT::LEFT:
            mEyePoint -= mU * speed;
            break;
        case MOVEMENT::RIGHT:
            mEyePoint += mU * speed;
            break;
        case MOVEMENT::UPWARD:
            mEyePoint += mWorldUp * speed;
            break;
        case MOVEMENT::DOWNWARD:
            mEyePoint -= mWorldUp * speed;
            break;
        default:
            break;
        }

        lookAt(mEyePoint + mViewDirection);
    }
};