#pragma once

#include "Scene.h"

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class SceneRoom : public Scene {
private:
    CameraFP mCam;
    gl::BatchRef mPlane;
    gl::TextureRef mPlaneTexture;
    gl::GlslProgRef mPlaneShader;

    gl::BatchRef mWall[4];
    gl::TextureRef mWallTexture;
    gl::GlslProgRef mWallShader;
    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
    double mTimeOffset;
    bool firstMouseMove = true;

public:
    ~SceneRoom() = default;
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};