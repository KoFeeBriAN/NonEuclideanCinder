#pragma once

#include "Scene.h"
#include "Portal.h"
#include <vector>

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class SceneTunnelVertical : public Scene {
private:
    CameraFP mCam;
    gl::TextureRef mFloorTex;
    gl::TextureRef mTunnelTex;
    gl::GlslProgRef mTexGlsl;
    gl::BatchRef mFloorBatch11, mFloorBatch12, mFloorBatch21, mFloorBatch22, mFloorBatch31, mFloorBatch32;
    std::vector<gl::BatchRef> mTunnelBatches1;
    std::vector<gl::BatchRef> mTunnelBatches2;
    std::vector<Portal*> mPortals;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;
    vec3 mLastCamPos;

    double mlastTime;
    double mTimeOffset;
    bool firstMouseMove = true;

public:
    ~SceneTunnelVertical() = default;
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};
