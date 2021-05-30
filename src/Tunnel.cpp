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

void Tunnel::setTexture(DataSourceRef source)
{
    mTexture = gl::Texture::create(loadImage(source));
}

void Tunnel::setGlsl(gl::GlslProgRef glsl)
{
    mGlsl = glsl;
}

void Tunnel::setCount(int count)
{
    mCount = count;
}

void Tunnel::setPosition(vec3 position)
{
    mPosition = position;
}

void Tunnel::draw()
{
    gl::color(mColor);
    mTexture->bind();
    for (auto batch : mBatches)
        batch->draw();
}