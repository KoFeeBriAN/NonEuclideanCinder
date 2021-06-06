#include "SceneTunnelSlope.h"
#include "Portal.h"

#include "cinder/CinderImGui.h"
#include "cinder/Log.h"
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

    mLastCamPos = mCam.getEyePoint();

    // set GLFW
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup texture
    mFloorTex = gl::Texture::create(loadImage(assets.at("checkerboard.png")));
    mTunnelTex = gl::Texture::create(loadImage(assets.at("rock-tunnel")));

    // setup glsl program
    mTexGlsl = gl::getStockShader(gl::ShaderDef().texture());

    // setup floor
    float slopeTheta = -22.5f;
    float slopeYPos = glm::sin(glm::radians(slopeTheta)) * 16;
    float slopeXPos = glm::cos(glm::radians(slopeTheta)) * 32;
    float slopeXPosCompensation = 1.216;

    auto floorTop1 = geom::Plane().size({ 32, 32 });
    auto floorSlope1 = geom::Plane().size({ 32, 32 }) >> geom::Rotate(glm::radians(slopeTheta), vec3(0, 0, 1)) >> geom::Translate(vec3(slopeXPos + slopeXPosCompensation, slopeYPos, 0));
    auto floorButtom1 = geom::Plane().size({ 32, 32 }) >> geom::Translate(vec3(2 * (slopeXPos + slopeXPosCompensation), 2 * slopeYPos, 0));
    auto floorTop2 = floorButtom1 >> geom::Translate(vec3(0, 64 - (2 * slopeYPos), 0));
    ;
    auto floorSlope2 = geom::Plane().size({ 32, 32 }) >> geom::Rotate(glm::radians(-slopeTheta), vec3(0, 0, 1)) >> geom::Translate(vec3(slopeXPos + slopeXPosCompensation, slopeYPos + 64, 0));
    auto floorBottom2 = floorTop1 >> geom::Translate(vec3(0, 64 + (2 * slopeYPos), 0));

    mFloorBatch11 = gl::Batch::create(floorTop1, mTexGlsl);
    mFloorBatch12 = gl::Batch::create(floorTop2, mTexGlsl);
    mFloorBatch21 = gl::Batch::create(floorSlope1, mTexGlsl);
    mFloorBatch22 = gl::Batch::create(floorSlope2, mTexGlsl);
    mFloorBatch31 = gl::Batch::create(floorButtom1, mTexGlsl);
    mFloorBatch32 = gl::Batch::create(floorBottom2, mTexGlsl);

    // setup TunnelNormal
    /*
    auto m1 = geom::Cube().size({ 32, 5, 1 }) >> geom::Rotate(glm::radians(slopeTheta), vec3(0, 0, 1)) >> geom::Translate(vec3(0, 3, -2.5)) >> geom::Translate(vec3((32 + slopeXPos + 0.6f) / 2, slopeYPos - 0.65f, 0));
    auto m2 = geom::Cube().size({ 32, 1, 6 }) >> geom::Rotate(glm::radians(slopeTheta), vec3(0, 0, 1)) >> geom::Translate(vec3(0, 6, 0)) >> geom::Translate(vec3(((32 + slopeXPos + 0.6f) / 2) + 1.15f, slopeYPos - 0.65f - 0.23f, 0));
    */
    //auto m1 = geom::Cube().size({ 32, 5, 1 }) >> geom::Rotate(glm::radians(-slopeTheta / 2), vec3(0, 0, 1)) >> geom::Scale(vec3(sin(slopeTheta / 2), cos(slopeTheta / 2), 1)) >> geom::Rotate(glm::radians(-45.0f), vec3(0, 0, 1)) >> geom::Scale(vec3(sqrt(2) / sin(slopeTheta / 2), sqrt(2), 1));
    mat4 shear = mat4(1.0, glm::radians(slopeTheta) - 0.0215, 0.0, 0.0, // 1. column
        0.0, 1.0, 0.0, 0.0, // 2. column
        0.0, 0.0, 1.0, 0.0, // 3. column
        0.0, 0.0, 0.0, 1.0); // 4. column
    auto m11 = geom::Cube().size({ slopeXPos, 5, 1 }) >> geom::Translate(vec3(0, 0, -2.5)) >> geom::Transform(shear) >> geom::Translate(vec3(((32 + slopeXPos) / 2), slopeYPos + 2.50f, 0));
    auto m12 = geom::Cube().size({ slopeXPos, 1, 6 }) >> geom::Translate(vec3(0, 3, 0)) >> geom::Transform(shear) >> geom::Translate(vec3((32 + slopeXPos) / 2, slopeYPos + 2.50f, 0));
    auto m13 = m11 >> geom::Translate(vec3(0, 0, 5));
    mTunnelBatches1.push_back(gl::Batch::create(m11, mTexGlsl));
    mTunnelBatches1.push_back(gl::Batch::create(m12, mTexGlsl));
    mTunnelBatches1.push_back(gl::Batch::create(m13, mTexGlsl));

    // setup TunnelLong
    shear = mat4(1.0, -(glm::radians(slopeTheta) - 0.0215), 0.0, 0.0, // 1. column
        0.0, 1.0, 0.0, 0.0, // 2. column
        0.0, 0.0, 1.0, 0.0, // 3. column
        0.0, 0.0, 0.0, 1.0); // 4. column
    auto m21 = geom::Cube().size({ slopeXPos, 5, 1 }) >> geom::Translate(vec3(0, 0, -2.5)) >> geom::Transform(shear) >> geom::Translate(vec3(((32 + slopeXPos) / 2), slopeYPos + 2.50f + 64, 0));
    auto m22 = geom::Cube().size({ slopeXPos, 1, 6 }) >> geom::Translate(vec3(0, 3, 0)) >> geom::Transform(shear) >> geom::Translate(vec3((32 + slopeXPos) / 2, slopeYPos + 2.50f + 64, 0));
    auto m23 = m21 >> geom::Translate(vec3(0, 0, 5));
    mTunnelBatches2.push_back(gl::Batch::create(m21, mTexGlsl));
    mTunnelBatches2.push_back(gl::Batch::create(m22, mTexGlsl));
    mTunnelBatches2.push_back(gl::Batch::create(m23, mTexGlsl));

    // Initialize portals
    mPortals.push_back(new Portal(mCam, vec3(16, 2.5, 0), Portal::NORMAL_DIR::NEG_X));
    mPortals.push_back(new Portal(mCam, vec3((slopeXPos + slopeXPosCompensation) * 2 - 16, 2 * slopeYPos + 2.5, 0), Portal::NORMAL_DIR::NEG_X));
    mPortals.push_back(new Portal(mCam, vec3((slopeXPos + slopeXPosCompensation) * 2 - 16, 2 * slopeYPos + 2.5, 0), Portal::NORMAL_DIR::X));
    mPortals.push_back(new Portal(mCam, vec3(16, 2.5, 0), Portal::NORMAL_DIR::X));
    //mPortals[0]->setOrigin(vec3(16, 2.5, 0));
    //mPortals[0]->setNormalDirection(Portal::NORMAL_DIR::NEG_X);

    //mPortals[1]->setOrigin(vec3((slopeXPos + slopeXPosCompensation) * 2 - 16, 2 * slopeYPos + 2.5, 0));
    //mPortals[1]->setNormalDirection(Portal::NORMAL_DIR::NEG_X);
    mPortals[0]->setSize(vec2(5, 4));
    mPortals[1]->setSize(vec2(5, 4));
    mPortals[2]->setSize(vec2(5, 4));
    mPortals[3]->setSize(vec2(5, 4));
    /*
    auto portal11 = new Portal(mCam, vec3(15.998, 3, 0), Portal::NORMAL_DIR::NEG_X);
    auto portal12 = new Portal(mCam, vec3(15.998, slopeYPos + 64 - 3.1225, 0), Portal::NORMAL_DIR::X);
    portal11->setSize(vec2(6, 6));
    portal12->setSize(vec2(6, 6));
    mPortals.push_back(portal11);
    //mPortals.push_back(new Portal(mCam, vec3(2 * slopeXPos + slopeXPosCompensation * 2, 2 * slopeYPos + 5, 0), Portal::NORMAL_DIR::X));
    mPortals.push_back(portal12);
    //mPortals.push_back(new Portal(mCam, vec3(12, 5, 0), Portal::NORMAL_DIR::X));
    */
    // Linked Portals
    mPortals[0]->setLinkedPortal(*mPortals[1]);
    mPortals[1]->setLinkedPortal(*mPortals[0]);
    mPortals[2]->setLinkedPortal(*mPortals[3]);
    mPortals[3]->setLinkedPortal(*mPortals[2]);

    for (auto& portal : mPortals)
        portal->setPlayerCamera(mCam);
    for (auto& portal : mPortals)
        portal->setup();

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

    for (auto& portal : mPortals)
        portal->update();

    for (auto& portal : mPortals) {
        if (portal->isIntersect(mLastCamPos, mCam.getEyePoint())) {
            portal->warp(mCam);
            break;
        }
    }

    mLastCamPos = mCam.getEyePoint();

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
    ImGui::Text("G - TomFloorBatch32->draw();gle fullscreen mode");
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

    gl::enableStencilTest();

    for (int i = 0; i < mPortals.size(); ++i) {
        gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        gl::depthMask(GL_FALSE);
        gl::stencilFunc(GL_NEVER, 0, 0xFF);
        gl::stencilOp(GL_INCR, GL_KEEP, GL_KEEP);
        gl::clear(GL_STENCIL_BUFFER_BIT);

        mPortals[i]->draw();

        gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        gl::depthMask(GL_TRUE);
        gl::stencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        gl::stencilFunc(GL_LEQUAL, 1, 0xFF);
        gl::pushViewMatrix();

        mat4 newView = Portal::getNewViewMatrix(gl::getViewMatrix(), mPortals[i]->getModelMatrix(), mPortals[i]->getLinkedPortal()->getModelMatrix());
        gl::setViewMatrix(newView);
        //
        if (i % 2 == 0) {
            mTunnelTex->bind();
            for (auto batch : mTunnelBatches1)
                batch->draw();

            mFloorTex->bind();
            mFloorBatch11->draw();
            mFloorBatch21->draw();
            mFloorBatch31->draw();
        } else {
            if (i == 3) {
                mFloorTex->bind();
                mFloorBatch31->draw();
            } else {
                mFloorTex->bind();
                mFloorBatch11->draw();
                mFloorBatch21->draw();
                mFloorBatch31->draw();
            }
        }
        //
        gl::popViewMatrix();
    }

    gl::disableStencilTest();

    gl::clear(GL_DEPTH_BUFFER_BIT);
    gl::colorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    gl::setMatrices(mCam);
    for (auto portal : mPortals)
        portal->draw();
    gl::colorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    mTunnelTex->bind();
    for (auto batch : mTunnelBatches1)
        batch->draw();

    mFloorTex->bind();
    mFloorBatch11->draw();
    mFloorBatch21->draw();
    mFloorBatch31->draw();
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
