#include "SceneRoom.h"
#include "Portal.h"

#include "cinder/CinderImGui.h"
#include "cinder/Log.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/wrapper.h"
#include <utility>

using namespace ci;

void SceneRoom::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Initialize ImGui
    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 20, 0 }); // set camera position
    mCam.lookAt({ 1., 0., 0. }); // set view direction
    mCam.toggleFloating();

    // Setup Parameter
    mRoomSize = glm::vec3(100., 50., 100.);

    // Setup Plane
    auto fmt = gl::Texture::Format();
    fmt.setWrap(GL_REPEAT, GL_REPEAT);
    fmt.enableMipmapping(true);
    fmt.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    mFloorTexture = gl::Texture::create(loadImage(assets.at("checkerboard.png")), fmt);

    mFloorShader = gl::getStockShader(gl::ShaderDef().texture(mFloorTexture));

    auto plane = geom::Plane().size({ mRoomSize.x, mRoomSize.z });

    mFloor = gl::Batch::create(plane, mFloorShader);

    // Setup Wall
    mWallThickness = 1.0;

    mWallTexture = gl::Texture::create(loadImage(assets.at("rock-toon.jpg")));

    mWallShader = gl::getStockShader(gl::ShaderDef().texture(mWallTexture));

    mWalls.push_back(gl::Batch::create(geom::Cube().size({ mWallThickness, mRoomSize.y, mRoomSize.z }) >> geom::Transform(geom::Translate(vec3(mRoomSize.x / 2, mRoomSize.y / 2, 0.))), mWallShader));
    mWalls.push_back(gl::Batch::create(geom::Cube().size({ mWallThickness, mRoomSize.y, mRoomSize.z }) >> geom::Transform(geom::Translate(vec3(-mRoomSize.x / 2, mRoomSize.y / 2, 0.))), mWallShader));
    mWalls.push_back(gl::Batch::create(geom::Cube().size({ mRoomSize.x, mRoomSize.y, mWallThickness }) >> geom::Transform(geom::Translate(vec3(0., mRoomSize.y / 2, mRoomSize.z / 2))), mWallShader));
    mWalls.push_back(gl::Batch::create(geom::Cube().size({ mRoomSize.x, mRoomSize.y, mWallThickness }) >> geom::Transform(geom::Translate(vec3(0., mRoomSize.y / 2, -mRoomSize.z / 2))), mWallShader));

    // Setup Portal Doors
    auto portalShader = gl::getStockShader(gl::ShaderDef().color());
    mPortalSize = vec2(25., 12.);

    mPortals.resize(2);
    mPortals[0].id = 0;
    mPortals[0].size = mPortalSize;
    mPortals[0].origin = vec3(mRoomSize.x / 2 - mWallThickness / 2 - 0.2, mRoomSize.y / 2 - mPortalSize.x / 2, 10.);
    mPortals[0].normal = vec3(-1, 0, 0);
    mPortals[0].batch = gl::Batch::create(
        geom::Plane()
                .normal(mPortals[0].normal)
                .size(mPortals[0].size)
                .origin(mPortals[0].origin)
            >> geom::Constant(geom::COLOR, ColorAf::black()),
        portalShader);
    mPortals[1].id = 1;
    mPortals[1].size = mPortalSize;
    mPortals[1].origin = vec3(-mRoomSize.x / 2 + mWallThickness / 2 + 0.2, mRoomSize.y / 2 - mPortalSize.x / 2, -10.0);
    mPortals[1].normal = vec3(1, 0, 0);
    mPortals[1].batch = gl::Batch::create(
        geom::Plane()
            .normal(mPortals[1].normal)
            .size(mPortals[1].size)
            .origin(mPortals[1].origin)
            >> geom::Constant(geom::COLOR, ColorAf::white()),
        portalShader);

    // Pair the portals
    mPortalPairs[0] = 1;
    mPortalPairs[1] = 0;
}

void SceneRoom::update(double currentTime)
{
    const vec3& viewDir = mCam.getViewDirection();
    const vec3& camPos = mCam.getEyePoint();

    // Debug UI
    ImGui::Begin("Debug panel");
    ImGui::Text("Camera property");
    ImGui::Text("View direction {%.2f, %.2f, %.2f}", viewDir.x, viewDir.y, viewDir.z);
    ImGui::Text("Position {%.2f, %.2f, %.2f}", camPos.x, camPos.y, camPos.z);
    ImGui::Text("Elapsed time:%.1f second", mlastTime);
    ImGui::SliderFloat("Camera sensitivity", &mCam.mMouseSensitivity, 0.001, 0.3);
    ImGui::End();

    // Update time logic
    mTimeOffset = currentTime - mlastTime;
    mlastTime = currentTime;

    // Poll for inputs
    processInput();

    // Collision logic
    for (auto& portal : mPortals)
        if (isCollidePortal(portal)) {
            collidePortal(portal);
            CI_LOG_D("wrap!");
        }
}

void SceneRoom::draw()
{
    // Gray Background
    gl::clear(Color::gray(0.2f));

    // Set up the camera
    gl::ScopedMatrices push;
    gl::setMatrices(mCam); // set matrix scene to match the camera

    // Enable depth buffer
    gl::ScopedDepth depth(true);

    // Draw Walls
    mWallTexture->bind();
    for (auto& wall : mWalls) {
        wall->draw();
    }

    // Draw Plane Floor
    mFloorTexture->bind();
    mFloor->draw();

    // Draw Portals
    for (auto& portal : mPortals) {
        portal.batch->draw();
    }
}

Camera* SceneRoom::getCamera()
{
    return &mCam;
}

void SceneRoom::handleKeyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_f)
        mCam.toggleFreeze(mGlfwWindowRef);
    if (event.getCode() == KeyEvent::KEY_t)
        mCam.toggleFloating();
    if (event.getCode() == KeyEvent::KEY_h)
        mCam.toggleHiddenCursor(mGlfwWindowRef);
}

void SceneRoom::handleMouseMove(MouseEvent event)
{
    if (firstMouseMove) {
        lastPos = event.getPos();
        firstMouseMove = false;
    } else {
        lastPos = currentPos;
    }

    currentPos = event.getPos();
    ivec2 offset = currentPos - lastPos;
    mCam.processMouse(offset.x, offset.y);
}

void SceneRoom::processInput()
{
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_W) == GLFW_PRESS)
        mCam.move(MOVEMENT::FORWARD, mTimeOffset);
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_S) == GLFW_PRESS)
        mCam.move(MOVEMENT::BACKWARD, mTimeOffset);
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_A) == GLFW_PRESS)
        mCam.move(MOVEMENT::LEFT, mTimeOffset);
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_D) == GLFW_PRESS)
        mCam.move(MOVEMENT::RIGHT, mTimeOffset);
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_SPACE) == GLFW_PRESS)
        mCam.move(MOVEMENT::UPWARD, mTimeOffset);
    if (glfwGetKey(mGlfwWindowRef, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        mCam.move(MOVEMENT::DOWNWARD, mTimeOffset);
}

bool SceneRoom::isCollidePortal(const Portal& portal)
{
    return  portal.inside(mCam.getEyePoint()) && portal.distance(mCam.getEyePoint()) <= 0.2f;
}

void SceneRoom::collidePortal(const Portal& portal)
{
    // Get the exit portal
    auto exitPortal = mPortals[mPortalPairs[portal.id]];

    // Move cam's position
    mCam.setEyePoint(exitPortal.origin + 1.0f * exitPortal.normal);
}
