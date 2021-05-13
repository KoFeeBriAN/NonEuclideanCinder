#include "CameraFP.h"

using namespace ci;
using namespace std;

void CameraFP::processMouse(float xoffset, float yoffset)
{
    if (mFrozen)
        return;

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

void CameraFP::move(MOVEMENT movement, double timeOffset)
{
    if (mFrozen)
        return;

    float currentY = mEyePoint.y;

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

    if (!mFloating)
        mEyePoint.y = currentY;

    lookAt(mEyePoint + mViewDirection);
}

void CameraFP::freeze(GLFWwindow* window)
{
    mFrozen = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void CameraFP::unfreeze(GLFWwindow* window)
{
    mFrozen = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CameraFP::toggleFreeze(GLFWwindow* window)
{
    if (mFrozen)
        unfreeze(window);
    else
        freeze(window);
}

void CameraFP::toggleFloating()
{
    mFloating = !mFloating;
}