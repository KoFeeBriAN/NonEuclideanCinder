#include "Portal.h"
#include "CameraFP.h"

#include "cinder/Log.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include <string>

using namespace ci;

Portal::Portal(const CameraFP& playerCam, const vec3& origin, const vec3& dest, const vec3& normal)
    : mOrigin(origin)
    , mDestination(dest)
    , mNormal(normal)
{
    mPortalCamera = new CameraFP(playerCam);
    mPlayerCamera = &playerCam;
    updateModelMatrix();
}

void Portal::setup()
{
    // Initialize Camera
    mPortalCamera->setEyePoint(mDestination);

    auto plane = geom::Plane();
    auto shader = gl::getStockShader(gl::ShaderDef().color());

    mBatch = gl::Batch::create(plane, shader);
}

void Portal::update()
{
    // mModelMatrix = glm::rotate(glm::mat4(1), glm::radians(), glm::vec3(0, 1, 0));
}

void Portal::draw()
{
    gl::pushMatrices();
    gl::setModelMatrix(mModelMatrix);
    mBatch->draw();
    gl::popMatrices();
}

void Portal::setPlayerCamera(const CameraFP& camera)
{
    mPlayerCamera = &camera;
}

CameraFP* Portal::getPortalCamera()
{
    return mPortalCamera;
}

void Portal::updateModelMatrix()
{
    mModelMatrix = glm::mat4(1.0);
    mModelMatrix = glm::translate(mModelMatrix, vec3(mOrigin));
    mModelMatrix = glm::scale(mModelMatrix, vec3(mSize, 1));
    // hard code normal for now
    mModelMatrix = glm::rotate(mModelMatrix, glm::radians(90.0f), vec3(1, 0, 0));
}
