#pragma once
#include "cinder/Camera.h"
#include "glfw/glfw3.h"

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
    bool mFrozen = false;

public:
    void processMouse(float xoffset, float yoffset);
    void move(MOVEMENT movement, double timeOffset);
    void freeze(GLFWwindow* window);
    void unfreeze(GLFWwindow* window);
    void toggleFreeze(GLFWwindow* window);
};
