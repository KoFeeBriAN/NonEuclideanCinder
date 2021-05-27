#pragma once

#include "Scene.h"

#include "CameraFP.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include <vector>

using namespace ci;

class SceneRoom : public Scene {
private:
    CameraFP mCam;
    glm::vec3 mRoomSize;
    gl::BatchRef mFloor;
    gl::TextureRef mFloorTexture;
    gl::GlslProgRef mFloorShader;

    std::vector<gl::BatchRef> mWalls;
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