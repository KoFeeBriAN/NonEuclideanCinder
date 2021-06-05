#include "Portal.h"
#include "CameraFP.h"

#include "cinder/Log.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include <string>

using namespace ci;

Portal::Portal(const CameraFP& playerCam, const vec3& origin, const NORMAL_DIR& dir)
    : mOrigin(origin)
    , mNormDir(dir)
{
    mPortalCamera = new CameraFP(playerCam);
    mPlayerCamera = &playerCam;

    switch (mNormDir) {
    case NORMAL_DIR::X:
        mNormal = vec3(1, 0, 0);
        mUp = vec3(0, 1, 0);
        mRight = vec3(0, 0, -1);
        break;
    case NORMAL_DIR::Z:
        mNormal = vec3(0, 0, 1);
        mUp = vec3(0, 1, 0);
        mRight = vec3(1, 0, 0);
        break;
    case NORMAL_DIR::Y:
    default:
        mNormal = vec3(0, 1, 0);
        mUp = vec3(0, 0, -1);
        mRight = vec3(1, 0, 0);
    }

    updateModelMatrix();
}

void Portal::setup()
{
    // Initialize Camera
    auto plane = geom::Plane();

    auto shader = gl::getStockShader(gl::ShaderDef().color());

    mBatch = gl::Batch::create(plane, shader);
}

void Portal::update()
{
}

void Portal::draw()
{
    gl::pushMatrices();
    gl::setModelMatrix(mModelMatrix);
    gl::enableFaceCulling(true);
    gl::color(Color(1, 0, 0));
    mBatch->draw();
    gl::enableFaceCulling(false);
    gl::popMatrices();
}

void Portal::setPlayerCamera(const CameraFP& camera)
{
    mPlayerCamera = &camera;
}

void Portal::setLinkedPortal(Portal& portal)
{
    mLinkedPortal = &portal;
}

mat4 Portal::getNewViewMatrix(const mat4& curView, const mat4& curModel, const mat4& dstModel)
{
    return curView * curModel * glm::inverse(dstModel);
}

CameraFP* Portal::getPortalCamera()
{
    return mPortalCamera;
}

mat4 Portal::getModelMatrix() const
{
    return mModelMatrix;
}

Portal* Portal::getLinkedPortal()
{
    return mLinkedPortal;
}

void Portal::updateModelMatrix()
{
    mModelMatrix = glm::mat4(1.0);
    mModelMatrix = glm::translate(mModelMatrix, vec3(mOrigin));

    switch (mNormDir) {
    case NORMAL_DIR::X:
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(0, 0, 1));
        break;
    case NORMAL_DIR::Z:
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(1, 0, 0));
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(0, 1, 0));
        break;
    case NORMAL_DIR::Y:
    default:
        break;
    }

    mModelMatrix = glm::scale(mModelMatrix, vec3(mSize.x, 1.0, mSize.y));
}

bool Portal::isIntersect(const vec3& la, const vec3& lb)
{
    if (la == lb)
        return 0;
    vec4 p[4] = { vec4(mOrigin + mSize.x * mUp - mSize.y * mRight, 1.0),
        vec4(mOrigin + mSize.x * mUp + mSize.y * mRight, 1.0),
        vec4(mOrigin - mSize.x * mUp - mSize.y * mRight, 1.0),
        vec4(mOrigin - mSize.x * mUp + mSize.y * mRight, 1.0) };
    // check intersection
    for (int i = 0; i < 2; ++i) {
        vec3 tuv = glm::inverse(glm::mat3(
                       vec3(la.x - lb.x, la.y - lb.y, la.z - lb.z),
                       vec3(p[i + 1].x - p[i].x, p[i + 1].y - p[i].y, p[i + 1].z - p[i].z),
                       vec3(p[i + 2].x - p[i].x, p[i + 2].y - p[i].y, p[i + 2].z - p[i].z)))
            * vec3(la.x - p[i].x, la.y - p[i].y, la.z - p[i].z);
        float t = tuv.x, u = tuv.y, v = tuv.z;
        if (t >= 0 - 1e-3 && t <= 1 + 1e-3) {
            if (u >= 0 - 1e-3 && u <= 1 + 1e-3 && v >= 0 - 1e-3 && v <= 1 + 1e-3 && (u + v) <= 1 + 1e-3)
                return 1;
        }
    }
    return 0;
}

void Portal::warp(CameraFP& camera)
{
    float offset = 0.7f;
    vec3 destPos = mLinkedPortal->mOrigin;
    vec3 destNorm = mLinkedPortal->mNormal;

    // Update Camera Position
    camera.setEyePoint(destPos + offset * destNorm);

    // mat4 newView = getNewViewMatrix(gl::getViewMatrix(), getModelMatrix(), getLinkedPortal()->getModelMatrix());
    // camera.setViewMatrix(newView);

    // Update Camera View
    float angle = glm::acos(glm::dot(mNormal, destNorm));
    vec3 newDir = glm::rotate(camera.getViewDirection(), -angle, vec3(0, 1, 0));

    camera.lookAt(destPos + newDir);
}

// mat3 Portal::rotateAlign(vec3 v1, vec3 v2)
// {
//     vec3 axis = cross( v1, v2 );

//     const float cosA = dot( v1, v2 );
//     const float k = 1.0f / (1.0f + cosA);

//     mat3 result( (axis.x * axis.x * k) + cosA,
//                  (axis.y * axis.x * k) - axis.z,
//                  (axis.z * axis.x * k) + axis.y,
//                  (axis.x * axis.y * k) + axis.z,
//                  (axis.y * axis.y * k) + cosA,
//                  (axis.z * axis.y * k) - axis.x,
//                  (axis.x * axis.z * k) - axis.y,
//                  (axis.y * axis.z * k) + axis.x,
//                  (axis.z * axis.z * k) + cosA
//                  );

//     return result;
// }