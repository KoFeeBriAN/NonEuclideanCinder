#pragma once

#include "Portal.h"
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
    gl::BatchRef mFloor2Batch;
    gl::BatchRef mFloor3Batch;
    gl::BatchRef mSkyboxBatch;

    // Portals
    std::vector<Portal> mPortals;

    //Tunnel
    Tunnel mShortTunnel;
    Tunnel mLongTunnel;
    Tunnel mImgShortTunnel;
    Tunnel mImgLongTunnel;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    vec3 mLastCamPos;

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

    void drawSceneObjects();
    void drawNonPortal(const glm::mat4& viewMat, const glm::mat4& projMat);
    void drawPortalRecursive(const glm::mat4& viewMat, const glm::mat4& projMat, size_t maxRecursiveLevel, size_t recursiveLevel);

    void drawPortalImages();

private:
    void processInput();
};
