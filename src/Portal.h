#pragma once

#define GLFW_INCLUDE_NONE

#include "cinder/Camera.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"

using namespace ci;

class Portal {
protected:
    vec2 mSize;
    vec3 mOrigin;
    vec3 mNormal;

    vec3 mDestination;
    vec3 mCameraViewDirection;

    Camera* mPortalCamera = new CameraPersp();
    Camera* mPlayerCamera;

    gl::BatchRef mBatch;
    gl::TextureRef mTexture;
    gl::GlslProgRef mShader;

public:
    Portal() = delete;
    Portal(const vec3& origin, const vec3& dest, const vec3& normal);
    ~Portal();
    void setup();
    void update();
    void draw();

    void setPlayerCamera(Camera& camera);
    Camera* getPortalCamera();
};