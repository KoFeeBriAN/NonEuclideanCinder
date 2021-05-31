#include "SceneRoom.h"

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

    // Initialize camera properties
    mCam.setEyePoint({ 0, 10, 0 }); // set camera position
    mCam.lookAt({ 1, 0, 0 }); // set view direction
    mCam.toggleFloating();

    // Initialize rooms
    mRooms.push_back(new Room({20, 10, 30}, {0, 0, 0}));
    mRooms.push_back(new Room({20, 10, 20}, {30, 0, 30}));

    for (auto &room: mRooms) room->setup(assets);
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

    for (auto &room: mRooms) room->draw();
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
