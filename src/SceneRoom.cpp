#include "SceneRoom.h"
#include "Portal.h"

#include "cinder/CinderImGui.h"
#include "cinder/Log.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/wrapper.h"
#include <utility>

using namespace ci;

SceneRoom::~SceneRoom()
{
    for (auto room : mRooms)
        delete room;
    for (auto portal : mPortals)
        delete portal;
}

void SceneRoom::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Initialize ImGui
    ImGui::Initialize();

    // Initialize camera properties

    mCam.setEyePoint({ 0, 5, -5 }); // set camera position
    mLastCamPos = mCam.getEyePoint();

    mCam.lookAt({ 0, 0, -1 }); // set view direction
    mCam.toggleFloating();

    // Initialize rooms
    mRooms.push_back(new Room({ 100, 20, 100 }, { 0, 0, 0 }));
    mRooms.push_back(new Room({ 100, 20, 100 }, { 100, 0, 0 }));
    mRooms.push_back(new Room({ 100, 20, 100 }, { 0, 0, 100 }));
    mRooms[1]->setRotateRoom(-90.0f);
    mRooms[2]->setRotateRoom(90.0f);

    for (auto& room : mRooms)
        room->setup(assets);

    mRooms[1]->setFloorTexture(gl::Texture::create(loadImage(assets.at("wood-toon"))));
    mRooms[1]->setWallTexture(gl::Texture::create(loadImage(assets.at("rock-tunnel"))));

    // Initialize portals
    mPortals.push_back(new Portal(mCam));
    mPortals.push_back(new Portal(mCam));
    mPortals[0]->setOrigin(vec3(100 + 50 - 10, 10, 0 + 50 + 0.1));
    mPortals[0]->setNormalDirection(Portal::NORMAL_DIR::NEG_Z);

    mPortals[1]->setOrigin(vec3(0 + 50 + 0.1, 10, 100 + 50 - 10));
    mPortals[1]->setNormalDirection(Portal::NORMAL_DIR::Z);
    mPortals[0]->setSize({ 20, 20 });
    mPortals[1]->setSize({ 20, 20 });

    // Linked Portals
    mPortals[0]->setLinkedPortal(*mPortals[1]);
    mPortals[1]->setLinkedPortal(*mPortals[0]);

    for (auto& portal : mPortals)
        portal->setPlayerCamera(mCam);
    for (auto& portal : mPortals)
        portal->setup();

    auto teapot = geom::Teapot() >> geom::Translate(vec3(5, 5, 5));
    auto glsl = gl::getStockShader(gl::ShaderDef().color());
    mObject = gl::Batch::create(teapot, glsl);

    auto sphere = geom::Sphere() >> geom::Translate(vec3(13, 34, 8));
    auto glsl2 = gl::getStockShader(gl::ShaderDef().color());
    mObject2 = gl::Batch::create(sphere, glsl2);
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
#include "cinder/Log.h"
    // Update time logic
    mTimeOffset = currentTime - mlastTime;
    mlastTime = currentTime;

    // Poll for inputs
    processInput();

    // Portal update
    for (auto& portal : mPortals)
        portal->update();

    for (auto& portal : mPortals) {
        if (portal->isIntersect(mLastCamPos, mCam.getEyePoint())) {
            portal->warp(mCam);
            break;
        }
    }

    // Update Last Position
    mLastCamPos = mCam.getEyePoint();
}

void SceneRoom::draw()
{
    // Gray Background
    gl::clear(Color::gray(0.2f));
    gl::setMatrices(mCam); // set matrix scene to match the camera

    gl::enableStencilTest();

    for (auto& portal : mPortals) {
        gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        gl::depthMask(GL_FALSE);
        gl::stencilFunc(GL_NEVER, 0, 0xFF);
        gl::stencilOp(GL_INCR, GL_KEEP, GL_KEEP);
        gl::clear(GL_STENCIL_BUFFER_BIT);

        portal->draw();

        gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl::depthMask(GL_TRUE);
        gl::stencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        gl::stencilFunc(GL_LEQUAL, 1, 0xFF);
        gl::pushViewMatrix();

        mat4 newView = Portal::getNewViewMatrix(mCam.getViewMatrix(), portal->getModelMatrix(), portal->getLinkedPortal()->getModelMatrix());
        gl::setViewMatrix(newView);
        for (auto room : mRooms)
            room->draw();
        mObject->draw();
        mObject2->draw();
        gl::popViewMatrix();
    }

    gl::disableStencilTest();

    gl::clear(GL_DEPTH_BUFFER_BIT);
    gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    gl::setMatrices(mCam);
    for (auto portal : mPortals)
        portal->draw();
    gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    for (auto room : mRooms)
        room->draw();

    mObject->draw();
    mObject2->draw();
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