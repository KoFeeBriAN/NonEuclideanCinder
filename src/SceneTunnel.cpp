#include "SceneTunnel.h"

#include "cinder/CinderImGui.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/wrapper.h"

using namespace ci;

void SceneTunnel::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{

    // set GLFW
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup texture
    mFloorTex = gl::Texture::create(loadImage(assets.at("checkerboard.png")));
    mTunnelTex = gl::Texture::create(loadImage(assets.at("rock-tunnel")));
    mSkyboxTex = gl::TextureCubeMap::create(loadImage(assets.at("galaxy-texture")),
        gl::TextureCubeMap::Format().mipmap());

    // setup glsl program
    auto textureGlsl = gl::getStockShader(gl::ShaderDef().texture());
    mColorGlsl = gl::getStockShader(gl::ShaderDef().color());
    auto skyboxGlsl = gl::GlslProg::create(assets.at("skybox.vert"), assets.at("skybox.frag"));

    // setup skybox
    mSkyboxBatch = gl::Batch::create(geom::Cube().size(vec3(300)), skyboxGlsl);
    mSkyboxBatch->getGlslProg()->uniform("uCubeMapTex", 0);

    // setup floor
    mFloorBatch = gl::Batch::create(geom::Plane().size({ 69, 69 }), textureGlsl);

    // setup warp plane
    auto door = geom::Plane();
    door.origin({ mDoorPos[0], mDoorPos[1], mDoorPos[2] });
    door.size({ mDoorSize[0], mDoorSize[1] });
    door.normal({ mDoorNormal[0], mDoorNormal[1], mDoorNormal[2] });
    mDoorBatch = gl::Batch::create(door, mColorGlsl);

    // setup Tunnel1
    auto m1 = geom::Cube().size({ 1, 6, 10 }) >> geom::Translate(vec3(-10, 3, -5));
    auto m2 = geom::Cube().size({ 6.5, 1, 10 }) >> geom::Translate(vec3(-7.25, 6.5, -5));
    auto m3 = m1 >> geom::Translate(vec3(5.5, 0, 0));
    mTunnelBatches.push_back(gl::Batch::create(m1, textureGlsl));
    mTunnelBatches.push_back(gl::Batch::create(m2, textureGlsl));
    mTunnelBatches.push_back(gl::Batch::create(m3, textureGlsl));

    // setup Tunnel2
    auto n1 = geom::Cube().size({ 1, 6, 30 }) >> geom::Translate(vec3(10, 3, -15));
    auto n2 = geom::Cube().size({ 6.5, 1, 30 }) >> geom::Translate(vec3(12.75, 6.5, -15));
    auto n3 = n1 >> geom::Translate(vec3(5.5, 0, 0));
    mTunnelBatches.push_back(gl::Batch::create(n1, textureGlsl));
    mTunnelBatches.push_back(gl::Batch::create(n2, textureGlsl));
    mTunnelBatches.push_back(gl::Batch::create(n3, textureGlsl));

    // setup dept wall
    auto wall = geom::Plane();
    wall.size(vec2(8, 20));
    wall.normal(vec3(-1, 0, 0));
    wall.origin(vec3(-10.51, 3, -20));
    mWallBatch = gl::Batch::create(wall, mColorGlsl);

    // setup illution Tunnel
    auto mx1 = m1 >> geom::Translate(vec3(0, 0, -10));
    auto mx2 = m2 >> geom::Translate(vec3(0, 0, -10));
    auto mx3 = m3 >> geom::Translate(vec3(0, 0, -10));
    auto mx4 = mx1 >> geom::Translate(vec3(0, 0, -10));
    auto mx5 = mx2 >> geom::Translate(vec3(0, 0, -10));
    auto mx6 = mx3 >> geom::Translate(vec3(0, 0, -10));
    mIllutionBatches.push_back(gl::Batch::create(mx1, textureGlsl));
    mIllutionBatches.push_back(gl::Batch::create(mx2, textureGlsl));
    mIllutionBatches.push_back(gl::Batch::create(mx3, textureGlsl));
    mIllutionBatches.push_back(gl::Batch::create(mx4, textureGlsl));
    mIllutionBatches.push_back(gl::Batch::create(mx5, textureGlsl));
    mIllutionBatches.push_back(gl::Batch::create(mx6, textureGlsl));

    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 3, 3 });
    mCam.lookAt({ 0, 1, 3 });
    mCam.toggleFloating();
}

void SceneTunnel::update(double currentTime)
{
    // update Door geom and batch
    auto door = geom::Plane();
    door.origin({ mDoorPos[0], mDoorPos[1], mDoorPos[2] });
    door.size({ mDoorSize[0], mDoorSize[1] });
    door.normal({ mDoorNormal[0], mDoorNormal[1], mDoorNormal[2] });
    mDoorBatch = gl::Batch::create(door, mColorGlsl);

    // Debug UI
    ImGui::Begin("Debug panel");
    ImGui::Text("Elapsed time:%.1f second", mlastTime);
    ImGui::Text("Door property");
    ImGui::InputFloat3("Normal", mDoorNormal, 2);
    ImGui::InputFloat3("Position", mDoorPos, 2);
    ImGui::InputFloat2("Size", mDoorSize, 2);
    ImGui::End();

    ImGui::Begin("Key biding");
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

void SceneTunnel::draw()
{
    gl::clear(Color::gray(0.2f));
    gl::clear(GL_STENCIL_BITS);
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::setMatrices(mCam);

    // draw skybox
    mSkyboxTex->bind();
    mSkyboxBatch->draw();

    // draw tunnel
    mTunnelTex->bind();
    for (auto batch : mTunnelBatches)
        batch->draw();

    // draw floor
    mFloorTex->bind();
    mFloorBatch->draw();

    // draw depth wall
    gl::color(Color(1, 0, 0));
    gl::colorMask(false, false, false, false);
    mWallBatch->draw();
    gl::colorMask(true, true, true, true);

    // draw illution tunnel
    mTunnelTex->bind();
    for (auto batch : mIllutionBatches)
        batch->draw();
}

Camera* SceneTunnel::getCamera()
{
    return &mCam;
}

void SceneTunnel::handleKeyDown(KeyEvent event)
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

void SceneTunnel::handleMouseMove(MouseEvent event)
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

void SceneTunnel::processInput()
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
