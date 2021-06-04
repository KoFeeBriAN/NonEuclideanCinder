#include "SceneTunnelSlope.h"

#include "cinder/Log.h"
#include "cinder/CinderImGui.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/wrapper.h"

#include <string>

using namespace ci;
using namespace std;

#define PI 3.141593

void SceneTunnelVertical::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    // set GLFW
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup texture
    mFloorTex = gl::Texture::create(loadImage(assets.at("checkerboard.png")));
    mTunnelTex = gl::Texture::create(loadImage(assets.at("rock-tunnel")));

    // setup glsl program
    mTexGlsl = gl::getStockShader(gl::ShaderDef().texture());

    // setup floor
    float slopeTheta = -22.5f;
    float slopeYPos = sin(glm::radians(slopeTheta)) * 16;
    float slopeXPos = cos(glm::radians(slopeTheta)) * 32;
    float slopeXPosCompensation = 1.216;
    slopeXPos += slopeXPosCompensation;

    auto floorTop = geom::Plane().size({ 32, 32 });
    auto floorSlope = geom::Plane().size({ 32, 32 }) >> geom::Rotate(glm::radians(slopeTheta), vec3(0, 0, 1)) >> geom::Translate(vec3(slopeXPos, slopeYPos, 0));
    auto floorButtom = geom::Plane().size({ 32, 32 })  >> geom::Translate(vec3(2 * slopeXPos, 2 * slopeYPos, 0));

    mFloorBatch1 = gl::Batch::create(floorTop, mTexGlsl);
    mFloorBatch2 = gl::Batch::create(floorSlope, mTexGlsl);
    mFloorBatch3 = gl::Batch::create(floorButtom, mTexGlsl);

    // setup Tunnel1
    auto m1 = geom::Cube().size({ 1, 6, 10 }) >> geom::Translate(vec3(-10, 3, -5));
    auto m2 = geom::Cube().size({ 6.5, 1, 10 }) >> geom::Translate(vec3(-7.25, 6.5, -5));
    auto m3 = m1 >> geom::Translate(vec3(5.5, 0, 0));
    mTunnelBatches.push_back(gl::Batch::create(m1, mTexGlsl));
    mTunnelBatches.push_back(gl::Batch::create(m2, mTexGlsl));
    mTunnelBatches.push_back(gl::Batch::create(m3, mTexGlsl));

    // setFullScreen(true);

    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 3, 3 });
    mCam.lookAt({ 0, 1, 3 });
    mCam.toggleFloating();
}

void SceneTunnelVertical::update(double currentTime)
{
    const vec3& viewDir = mCam.getViewDirection();
    const vec3& camPos = mCam.getEyePoint();

    // Debug UI
    ImGui::Begin("Debug panel");
    ImGui::Text("Camera property");
    ImGui::Text("View direction {%.2f, %.2f, %.2f}", viewDir.x, viewDir.y, viewDir.z);
    ImGui::Text("Position {%.2f, %.2f, %.2f}", camPos.x, camPos.y, camPos.z);
    ImGui::Text("Elapsed time:%.1f second", mlastTime);
    ImGui::Separator();
    ImGui::SliderFloat("Camera sensitivity", &mCam.mMouseSensitivity, 0.01, 0.3);
    ImGui::Separator();
    ImGui::Text("Key binding");
    ImGui::Text("W - Move forward");
    ImGui::Text("A - Move left");
    ImGui::Text("S - Move backward");
    ImGui::Text("D - Move right");
    ImGui::Text("Ctrl - Move downward");
    ImGui::Text("Space - Move upward");
    ImGui::Text("F - Freeze the camera");
    ImGui::Text("T - Toggle floating camera");
    ImGui::Text("G - Togle fullscreen mode");
    ImGui::Text("Esc - Close applicaiton");
    ImGui::End();

    // Update time logic
    mTimeOffset = currentTime - mlastTime;
    mlastTime = currentTime;

    // Poll for inputs
    processInput();
}

void SceneTunnelVertical::draw()
{
    gl::clear(Color::gray(0.2f));
    gl::setMatrices(mCam);

    mTunnelTex->bind();
    for (auto batch : mTunnelBatches)
        batch->draw();

    mFloorTex->bind();
    mFloorBatch1->draw();
    mFloorBatch2->draw();
    mFloorBatch3->draw();
}

Camera* SceneTunnelVertical::getCamera()
{
    return &mCam;
}

void SceneTunnelVertical::handleKeyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_f)
        mCam.toggleFreeze(mGlfwWindowRef);
    if (event.getCode() == KeyEvent::KEY_t)
        mCam.toggleFloating();

    // For testing
    if (event.getCode() == KeyEvent::KEY_i)
        mCam.move(MOVEMENT::FORWARD, mTimeOffset);
    if (event.getCode() == KeyEvent::KEY_k)
        mCam.move(MOVEMENT::BACKWARD, mTimeOffset);
    if (event.getCode() == KeyEvent::KEY_j)
        mCam.move(MOVEMENT::LEFT, mTimeOffset);
    if (event.getCode() == KeyEvent::KEY_l)
        mCam.move(MOVEMENT::RIGHT, mTimeOffset);
}

void SceneTunnelVertical::handleMouseMove(MouseEvent event)
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

void SceneTunnelVertical::processInput()
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
