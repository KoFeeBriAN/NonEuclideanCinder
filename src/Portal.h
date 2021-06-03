#pragma once

#define GLFW_INCLUDE_NONE

#include "CameraFP.h"

#include "cinder/Camera.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"

using namespace ci;

class Portal {
public:
    Portal() = delete;
    Portal(const CameraFP& playerCam, const vec3& origin, const vec3& dest, const vec3& normal);
    ~Portal();
    void setup();
    void update();
    void draw();

    void setPlayerCamera(const CameraFP& camera);
    static mat4 getNewViewMatrix(const mat4& curView, const mat4& curModel, const mat4& dstModel);
    CameraFP* getPortalCamera();
    mat4 getModelMatrix() const;

protected:
    vec2 mSize = vec2(3, 3);
    vec3 mOrigin;
    vec3 mNormal;

    vec3 mDestination;
    vec3 mCameraViewDirection;

    CameraFP* mPortalCamera;
    const CameraFP* mPlayerCamera;

    mat4 mModelMatrix;

    gl::BatchRef mBatch;
    gl::TextureRef mTexture;
    gl::GlslProgRef mShader;

    void updateModelMatrix();
};