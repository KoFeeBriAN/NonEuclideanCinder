#pragma once
#include "Scene.h"

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"

class SceneTest : public Scene {
private:
    CameraFP mCam;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
    double mTimeOffset;

public:
    void setup() override;
    void update() override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};