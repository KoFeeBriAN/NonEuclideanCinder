#pragma once

#define GLFW_INCLUDE_NONE

#include "CameraFP.h"

#include "cinder/Camera.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"

using namespace ci;

class Portal {
public:
    enum NORMAL_DIR { X,
        Y,
        Z,
        NEG_X,
        NEG_Y,
        NEG_Z };

    static mat4 getNewViewMatrix(const mat4& curView, const mat4& curModel, const mat4& dstModel);

    Portal() = delete;
    Portal(const CameraFP& playerCam, const vec3& origin = vec3(), const NORMAL_DIR& dir = NORMAL_DIR::X);
    ~Portal() = default;
    void setup();
    void update();
    void draw();

    void setSize(vec2 size);
    void setOrigin(vec3 origin);
    void setNormalDirection(NORMAL_DIR dir);
    void setPlayerCamera(const CameraFP& camera);
    void setLinkedPortal(Portal& portal);

    bool isIntersect(const vec3& la, const vec3& lb);
    void warp(CameraFP& camera);

    mat4 getModelMatrix() const;
    Portal* getLinkedPortal();

    glm::mat4 const clippedProjectMat(const glm::mat4& viewMat, glm::mat4 projMat) const;

protected:
    vec2 mSize = vec2(20, 20);
    vec3 mOrigin;

    NORMAL_DIR mNormDir;
    vec3 mNormal;
    vec3 mUp;
    vec3 mRight;

    vec3 mCameraViewDirection;

    Portal* mLinkedPortal;
    const CameraFP* mPlayerCamera;

    mat4 mModelMatrix;

    gl::BatchRef mBatch;
    gl::TextureRef mTexture;
    gl::GlslProgRef mShader;

    void updateModelMatrix();
};