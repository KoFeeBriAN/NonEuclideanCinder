#pragma once

#include "CameraFP.h"
#include "Portal.h"
#include "Room.h"
#include "Scene.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include <map>
#include <vector>

using namespace ci;

class SceneRoom : public Scene {
private:
    friend class Portal;
    // Camera Members
    CameraFP mCam;

    // Rooms
    std::vector<Room*> mRooms;

    // Portal Members
    vec2 mPortalSize;
    std::vector<Portal*> mPortals;

    // objects
    gl::BatchRef mObject;
    gl::BatchRef mObject2;

    ivec2 lastPos;
    ivec2 currentPos;

    vec3 mLastCamPos;

    double mlastTime;
    double mTimeOffset;
    bool firstMouseMove = true;

public:
    ~SceneRoom();
    void setup(const std::unordered_map<std::string, DataSourceRef>& assets) override;
    void update(double currentTime) override;
    void draw() override;
    Camera* getCamera() override;
    void handleMouseMove(MouseEvent event) override;
    void handleKeyDown(KeyEvent event) override;

private:
    void processInput();
};