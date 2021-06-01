#include "Portal.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include <string>

using namespace ci;

Portal::Portal(vec3 origin, vec3 dest, vec3 normal)
    : mOrigin(origin)
    , mDestination(dest)
    , mNormal(normal)
{
}

void Portal::setup()
{
    mSize = vec2({ 8, 5 });

    // Initialize Camera
    mPortalCamera->setEyePoint(mDestination);
    mPortalCamera->setAspectRatio(mSize.y / mSize.x);
    mPortalCamera->lookAt(mPortalCamera->getEyePoint() - mNormal);


    auto plane = geom::Plane().origin(mOrigin).size(mSize).normal(mNormal) >> geom::Translate(vec3({ 0, mSize[0] / 2, 0 }));
    auto shader = gl::getStockShader(gl::ShaderDef().color());

    mBatch = gl::Batch::create(plane, shader);
}

void Portal::update()
{
    CI_LOG_D(mPortalCamera->getEyePoint());
    CI_LOG_D(mPortalCamera->getViewDirection());
}

void Portal::draw()
{
    gl::color(Color(1, 0, 0));
    mBatch->draw();
}

void Portal::setPlayerCamera(Camera &camera)
{
    mPlayerCamera = &camera;
}

Camera* Portal::getPortalCamera()
{
    return mPortalCamera;
}