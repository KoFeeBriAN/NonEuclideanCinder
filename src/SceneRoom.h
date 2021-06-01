#pragma once

#include "Scene.h"
#include "Room.h"
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

    // Rooms
    std::vector<Room*> mRooms;

    // Portal Members
    vec2 mPortalSize;
    std::vector<Portal*> mPortals;

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

    void drawPortal(Portal *portal);


private:
    void processInput();
};