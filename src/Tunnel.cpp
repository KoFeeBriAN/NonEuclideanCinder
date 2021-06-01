#include "Tunnel.h"

using namespace ci;

void Tunnel::setupTunnel()
{
    // left wall
    auto left = geom::Cube().size(mThickness, mHeight, mLong)
        >> geom::Translate(mPosition + vec3(-(mWidth + mThickness) / 2, 0, -mLong / 2));
    // top wall
    auto top = geom::Cube().size(mWidth + 2 * mThickness, mThickness, mLong)
        >> geom::Translate(mPosition + vec3(0, (mHeight + mThickness) / 2, -mLong / 2));
    auto right = left >> geom::Translate(vec3(mWidth + mThickness, 0, 0));

    mBatches.reserve(mCount * 3);
    for (int i = 0; i < mCount; i++) {
        mBatches.push_back(gl::Batch::create(left, mGlsl));
        mBatches.push_back(gl::Batch::create(top, mGlsl));
        mBatches.push_back(gl::Batch::create(right, mGlsl));

        left = left >> geom::Translate(vec3(0, 0, -mLong));
        top = top >> geom::Translate(vec3(0, 0, -mLong));
        right = right >> geom::Translate(vec3(0, 0, -mLong));
    }
}

void Tunnel::setupSideWall()
{
    auto glsl = gl::getStockShader(gl::ShaderDef().color());

    auto left = geom::Plane();
    left.size(vec2(mHeight + mThickness + 0.01, mLong * (mCount - 1)));
    left.origin(mPosition + vec3(-mWidth / 2 - mThickness - 0.01, mThickness / 2.0, -mLong - (mCount - 1) * mLong / 2.0));
    left.normal(vec3(1, 0, 0));
    mWallBatches.push_back(gl::Batch::create(left, glsl));

    auto right = left >> geom::Translate(vec3(mWidth + 2 * mThickness + 0.02, 0, 0));
    mWallBatches.push_back(gl::Batch::create(right, glsl));
}

void Tunnel::setupFrontWall()
{
    auto glsl = gl::getStockShader(gl::ShaderDef().color());

    auto front = geom::Plane()
                     .size(vec2(mWidth, mHeight))
                     .origin(mPosition + vec3(0, 0, -mLong))
                     .normal(vec3(0, 0, 1));
    mWallBatches.push_back(gl::Batch::create(front, glsl));
}

void Tunnel::setTexture(const DataSourceRef& source)
{
    mTexture = gl::Texture::create(loadImage(source));
}

void Tunnel::setGlsl(const gl::GlslProgRef& glsl)
{
    mGlsl = glsl;
}

void Tunnel::setCount(int count)
{
    mCount = count;
}

void Tunnel::setPosition(const vec3& position)
{
    mPosition = position;
}

void Tunnel::draw()
{
    gl::color(mColor);
    mTexture->bind();
    for (int i = 0; i < 3; i++)
        mBatches[i]->draw();

    gl::colorMask(false, false, false, false);
    for (auto wallBatch : mWallBatches)
        wallBatch->draw();
    gl::colorMask(true, true, true, true);

    for (int i = 3; i < mBatches.size(); i++)
        mBatches[i]->draw();
}