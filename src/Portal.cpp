#include "Portal.h"
#include "CameraFP.h"

#include "cinder/Log.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include <string>

using namespace ci;

Portal::Portal(const CameraFP& playerCam, const vec3& origin, const NORMAL_DIR& dir)
{
    mPlayerCamera = &playerCam;
    setOrigin(origin);
    setNormalDirection(dir);

    updateModelMatrix();
}

void Portal::setup()
{
    // Initialize Camera
    auto plane = geom::Plane().size(mSize);

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

void Portal::setNormalDirection(Portal::NORMAL_DIR dir)
{
    mNormDir = dir;
    switch (mNormDir) {
    case NORMAL_DIR::X:
        mNormal = vec3(1, 0, 0);
        break;
    case NORMAL_DIR::NEG_X:
        mNormal = vec3(-1, 0, 0);
        break;
    case NORMAL_DIR::Z:
        mNormal = vec3(0, 0, 1);
        break;
    case NORMAL_DIR::NEG_Z:
        mNormal = vec3(0, 0, -1);
        break;
    // !not implement Y so do not use it!!!
    case NORMAL_DIR::Y:
    default:
        mNormal = vec3(1, 0, 0);
    }
    mRight = glm::normalize(::cross(mNormal, vec3(0, 1, 0))); // ERR when mNormal == vec3(0, 1, 0);
    mUp = glm::normalize(glm::cross(mRight, mNormal));
    updateModelMatrix();
}

void Portal::setSize(vec2 size)
{
    mSize = size;
    updateModelMatrix();
}

void Portal::setOrigin(vec3 origin)
{
    mOrigin = origin;
    updateModelMatrix();
}

mat4 Portal::getNewViewMatrix(const mat4& curView, const mat4& curModel, const mat4& dstModel)
{
    return curView * curModel
        * glm::rotate(mat4(1.0), glm::radians(180.0f), vec3(0, 1, 0))
        * glm::rotate(mat4(1.0), glm::radians(180.0f), vec3(0, 0, 1))
        * glm::inverse(dstModel);
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
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(-90.0f), vec3(0, 0, 1));
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(180.0f), vec3(0, 1, 0));
        // mModelMatrix = glm::scale(mModelMatrix, vec3(mSize.x / 2, 1.0, mSize.y / 2));
        break;
    case NORMAL_DIR::NEG_X:
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(0, 0, 1));
        // mModelMatrix = glm::scale(mModelMatrix, vec3(mSize.x / 2, 1.0, mSize.y / 2));
        break;
    case NORMAL_DIR::Z:
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(1, 0, 0));
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(0, 1, 0));
        // mModelMatrix = glm::scale(mModelMatrix, vec3(mSize.x / 2, 1.0, mSize.y / 2));
        break;
    case NORMAL_DIR::NEG_Z:
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(1, 0, 0));
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(0, 1, 0));
        mModelMatrix = glm::rotate(mModelMatrix, glm::radians(180.0f), vec3(1, 0, 0));
        // mModelMatrix = glm::scale(mModelMatrix, vec3(mSize.x / 2, 1.0, mSize.y / 2));
        break;
    case NORMAL_DIR::Y:
    default:
        break;
    }
}

bool Portal::isIntersect(const vec3& la, const vec3& lb)
{
    if (la == lb)
        return 0;
    if (glm::dot((lb - la), mNormal) > 0)
        return 0;
    vec4 p[4] = { vec4(mOrigin + mSize.x / 2 * mUp - mSize.y / 2 * mRight, 1.0),
        vec4(mOrigin + mSize.x / 2 * mUp + mSize.y / 2 * mRight, 1.0),
        vec4(mOrigin - mSize.x / 2 * mUp - mSize.y / 2 * mRight, 1.0),
        vec4(mOrigin - mSize.x / 2 * mUp + mSize.y / 2 * mRight, 1.0) };
    // check intersection
    for (int i = 0; i < 2; ++i) {
        vec3 tuv = glm::inverse(glm::mat3(
                       vec3(la.x - lb.x, la.y - lb.y, la.z - lb.z),
                       vec3(p[i + 1].x - p[i].x, p[i + 1].y - p[i].y, p[i + 1].z - p[i].z),
                       vec3(p[i + 2].x - p[i].x, p[i + 2].y - p[i].y, p[i + 2].z - p[i].z)))
            * vec3(la.x - p[i].x, la.y - p[i].y, la.z - p[i].z);
        float t = tuv.x, u = tuv.y, v = tuv.z;
        if (t >= 0 - 1e-1 && t <= 1 + 1e-1) {
            if (u >= 0 - 1e-1 && u <= 1 + 1e-1 && v >= 0 - 1e-1 && v <= 1 + 1e-1 && (u + v) <= 1 + 1e-1)
                return 1;
        }
    }
    return 0;
}

void Portal::warp(CameraFP& camera)
{
    float offset = .3f;
    vec3 destPos = mLinkedPortal->mOrigin;
    vec3 destNorm = mLinkedPortal->mNormal;

    // Update Camera View
    float angle = glm::acos(glm::dot(mNormal, destNorm));
    float sign = glm::cross(mNormal, destNorm).y;

    CI_LOG_D(angle);
    CI_LOG_D(sign);

    vec3 v = mOrigin - camera.getEyePoint();
    if (sign != 0)
        v = vec3(v.z, v.y, v.x);
    else if (angle == 0)
        v = vec3(-v.x, v.y, -v.z);

    // Update Camera Position
    // CI_LOG_D("mOrigin");
    // CI_LOG_D(mOrigin);
    // CI_LOG_D("destPos");
    // CI_LOG_D(destPos);
    // CI_LOG_D("curPos Prev");
    // CI_LOG_D(camera.getEyePoint());
    // CI_LOG_D("v1 destPos - mOrigin");
    // CI_LOG_D(v1);
    // CI_LOG_D("v mOrigin - curPos Prev");
    // CI_LOG_D(v);

    camera.setEyePoint(destPos - v + offset * destNorm);

    // CI_LOG_D("curPos Now");
    // CI_LOG_D(camera.getEyePoint());

    vec3 newDir = glm::rotate(camera.getViewDirection(), sign <= 0 ? glm::pi<float>() - angle : -angle, vec3(0, 1, 0));
    camera.lookAt(camera.getEyePoint() + newDir);

    updateModelMatrix();
}

glm::mat4 const Portal::clippedProjectMat(const glm::mat4& viewMat, glm::mat4 projMat) const
{
    glm::vec3 normal = mNormal;
    glm::vec4 clipPlane(normal, glm::dot(normal, mOrigin));
    glm::vec4 q;
    q.x = (glm::sign(clipPlane.x) + projMat[2][0]) / projMat[0][0];
    q.y = (glm::sign(clipPlane.y) + projMat[2][1]) / projMat[1][1];
    q.z = -1.0F;
    q.w = (1.0F + projMat[2][2]) / projMat[3][2];
    glm::vec4 c = clipPlane * (2.0f / glm::dot(clipPlane, q));
    projMat[0][2] = c.x;
    projMat[1][2] = c.y;
    projMat[2][2] = c.z + 1.0F;
    projMat[3][2] = c.w;
    return projMat;
}