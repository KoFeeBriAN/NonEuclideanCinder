#pragma once

#include "Scene.h"

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class SceneTest : public Scene {
private:
    CameraFP mCam;
    gl::BatchRef mSphere;
    gl::TextureRef mTexture;
    gl::GlslProgRef mGlsl;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
    double mTimeOffset;

public:
    void setup(std::vector<DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};