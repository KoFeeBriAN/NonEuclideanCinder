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

    // Initialize camera properties
    mCam.setEyePoint({ 0, 5, 0 }); // set camera position
    mCam.lookAt({ 0, 0, 0 }); // set view direction
    mCam.toggleFloating();

    // Initialize rooms
    mRooms.push_back(new Room({ 100, 10, 100 }, { 0, 0, 0 }));
    mRooms.push_back(new Room({ 100, 10, 100 }, { 0, 30, 0 }));
    // mRooms.push_back(new Room({100, 10, 100}, {100, 0, 100}));

    for (auto& room : mRooms)
        room->setup(assets);

    mRooms[1]->setFloorTexture(gl::Texture::create(loadImage(assets.at("wood-toon"))));
    mRooms[1]->setWallTexture(gl::Texture::create(loadImage(assets.at("rock-tunnel"))));

    // Initialize portals
    mPortals.push_back(new Portal(mCam, vec3(0, 4, 0), vec3(0, 20, 0), vec3(1, 0, 0)));
    mPortals.push_back(new Portal(mCam, vec3(10, 34, 5), vec3(0, 20, 0), vec3(0, 0, 1)));

    for (auto& portal : mPortals)
        portal->setPlayerCamera(mCam);
    for (auto& portal : mPortals)
        portal->setup();

    auto teapot = geom::Teapot() >> geom::Translate(vec3(5, 5, 5));
    auto glsl = gl::getStockShader(gl::ShaderDef().color());
    mObject = gl::Batch::create(teapot, glsl);
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

    // Portal update
    for (auto& portal : mPortals)
        portal->update();
}

void SceneRoom::draw()
{
    // Gray Background
    gl::clear(Color::gray(0.2f));
    gl::setMatrices(mCam); // set matrix scene to match the camera

    gl::enableStencilTest();

    gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    gl::depthMask(GL_FALSE);
    gl::stencilFunc(GL_NEVER, 0, 0xFF);
    gl::stencilOp(GL_INCR, GL_KEEP, GL_KEEP);
    gl::clear(GL_STENCIL_BUFFER_BIT);
    for (auto portal : mPortals)
        portal->draw();

    gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    gl::depthMask(GL_TRUE);
    gl::stencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    gl::stencilFunc(GL_LEQUAL, 1, 0xFF);
    gl::pushViewMatrix();
    mat4 newView = Portal::getNewViewMatrix(gl::getViewMatrix(), mPortals[0]->getModelMatrix(), mPortals[1]->getModelMatrix());
    gl::setViewMatrix(newView);
    for (auto room : mRooms)
        room->draw();
    gl::color(Color(1, 0, 0));
    mObject->draw();
    gl::popViewMatrix();

    gl::disableStencilTest();

    gl::clear(GL_DEPTH_BUFFER_BIT);
    gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    for (auto portal : mPortals)
        portal->draw();
    gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    for (auto room : mRooms)
        room->draw();
    gl::color(Color(1, 0, 0));
    mObject->draw();
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