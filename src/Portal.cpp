#include "Portal.h"
#include "CameraFP.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include <string>

using namespace ci;

Portal::Portal(const CameraFP& playerCam, vec3 origin, vec3 dest, vec3 normal)
    : mOrigin(origin)
    , mDestination(dest)
    , mNormal(normal)
{
    mPortalCamera = new CameraFP(playerCam);
}

void Portal::setup()
{
    mSize = vec2({ 8, 5 });

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

    mBatch->draw();
    gl::popMatrices();
}

void Portal::setPlayerCamera(Camera &camera)
{
    mPlayerCamera = &camera;
}

Camera* Portal::getPortalCamera()
{
    return mPortalCamera;
}