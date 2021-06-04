#pragma once

#define GLFW_INCLUDE_NONE

#include "CameraFP.h"

#include "cinder/Camera.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"

using namespace ci;

class Portal {
public:

    enum NORMAL_DIR { X, Y, Z };

    static mat4 getNewViewMatrix(const mat4& curView, const mat4& curModel, const mat4& dstModel);

    Portal() = delete;
    Portal(const CameraFP& playerCam, const vec3& origin, const NORMAL_DIR& dir);
    ~Portal();
    void setup();
    void update();
    void draw();

    void setSize(vec2 size);
    void setOrigin(vec3 origin);
    void setPlayerCamera(const CameraFP& camera);
    void setLinkedPortal(Portal& portal);
    
    bool isIntersect(const vec3& la, const vec3& lb);
    void warp(CameraFP& camera);

    CameraFP* getPortalCamera();
    mat4 getModelMatrix() const;
    Portal* getLinkedPortal();

protected:
    vec2 mSize = vec2(5, 3);
    vec3 mOrigin;

    NORMAL_DIR mNormDir;
    vec3 mNormal;
    vec3 mUp;
    vec3 mRight;

    vec3 mCameraViewDirection;

    CameraFP* mPortalCamera;
    Portal* mLinkedPortal;
    const CameraFP* mPlayerCamera;

    mat4 mModelMatrix;

    gl::BatchRef mBatch;
    gl::TextureRef mTexture;
    gl::GlslProgRef mShader;

    void updateModelMatrix();
    // mat3 rotateAlign(vec3 v1, vec3 v2);
};