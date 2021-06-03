#pragma once

#define GLFW_INCLUDE_NONE

#include "CameraFP.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/Camera.h"

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

    mat4 mModelMatrix;

    gl::BatchRef mBatch;
    gl::TextureRef mTexture;
    gl::GlslProgRef mShader;

public:
    Portal() = delete;
    Portal(const CameraFP& playerCam, vec3 origin, vec3 dest, vec3 normal);
    ~Portal();
    void setup();
    void update();
    void draw();

    void setPlayerCamera(Camera &camera);
    Camera* getPortalCamera();
};