#pragma once

#include "Scene.h"
#include "Portal.h"

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include <vector>
#include <map>

using namespace ci;

class SceneRoom : public Scene {
private:
    friend class Portal;
    // Camera Members
    CameraFP mCam;

    // Room Size vec3(width, high, long)
    glm::vec3 mRoomSize;

    // Floor Members
    gl::BatchRef mFloor;
    gl::TextureRef mFloorTexture;
    gl::GlslProgRef mFloorShader;

    // Wall Members
    float mWallThickness;
    std::vector<gl::BatchRef> mWalls;
    gl::TextureRef mWallTexture;
    gl::GlslProgRef mWallShader;

    // Portal Members
    vec2 mPortalSize;
    std::vector<Portal> mPortals;
    std::map<PortalId, PortalId> mPortalPairs;

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

public:
    bool isCollidePortal(const Portal &portal);
    void collidePortal(const Portal &portal);

private:
    void processInput();
};