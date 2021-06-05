#pragma once

#define GLFW_INCLUDE_NONE

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
    vec3 mTarget;
    float mPitch = 0;
    float mYaw = -90;
    float mMoveSpeed = 25;
    bool mFrozen = false;
    bool mHidden = false;
    bool mFloating = true;

public:
    float mMouseSensitivity = 0.2;

    void processMouse(float xoffset, float yoffset);
    void move(MOVEMENT movement, double timeOffset);
    void freeze(GLFWwindow* window);
    void unfreeze(GLFWwindow* window);
    void showCursor(GLFWwindow* window);
    void hideCursor(GLFWwindow* window);
    void toggleHiddenCursor(GLFWwindow* window);
    void toggleFreeze(GLFWwindow* window);
    void toggleFloating();

    vec3 getTarget() const;

    void setViewMatrix(mat4 viewMat);
};
