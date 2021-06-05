#pragma once

#include "Scene.h"
#include "Tunnel.h"
#include <vector>

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class SceneTunnelPortal : public Scene {
private:
    CameraFP mCam;
    gl::TextureRef mFloorTex;
    gl::TextureRef mTunnelTex;
    gl::TextureCubeMapRef mSkyboxTex;
    gl::GlslProgRef mColorGlsl;
    gl::BatchRef mFloorBatch;
    gl::BatchRef mImgFloorBatch;
    gl::BatchRef mSkyboxBatch;

    //Tunnel
    Tunnel mShortTunnel;
    Tunnel mLongTunnel;
    Tunnel mImgShortTunnel;
    Tunnel mImgLongTunnel;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
    double mTimeOffset;
    bool firstMouseMove = true;

public:
    ~SceneTunnelPortal() = default;
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};
