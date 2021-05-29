#pragma once

#include "Scene.h"
#include <vector>

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class SceneTunnel : public Scene {
private:
    CameraFP mCam;
    gl::TextureRef mFloorTex;
    gl::TextureRef mTunnelTex;
    gl::TextureCubeMapRef mSkyboxTex;
    gl::GlslProgRef mColorGlsl;
    gl::BatchRef mFloorBatch;
    gl::BatchRef mSkyboxBatch;
    gl::BatchRef mDoorBatch;
    std::vector<gl::BatchRef> mTunnelBatches;
    std::vector<gl::BatchRef> mIllutionBatches;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    // Debug door attribute
    float mDoorPos[3] = { -7.25f, 3.0f };
    float mDoorSize[2] = { 4.5f, 6.0f };
    float mDoorNormal[3] = { 0.0f, 0.0f, 1.0f };

    double mlastTime;
    double mTimeOffset;
    bool firstMouseMove = true;

public:
    ~SceneTunnel() = default;
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};
