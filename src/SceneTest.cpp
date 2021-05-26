#include "SceneTest.h"

#include "cinder/CinderImGui.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/wrapper.h"

using namespace ci;

void SceneTest::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    // prepare batch program
    auto img = loadImage(assets.at("checkerboard.png"));
    mTexture = gl::Texture::create(img);
    mTexture->bind();
    auto shader = gl::ShaderDef().texture().lambert();
    mGlsl = gl::getStockShader(shader);
    auto sphere = geom::Sphere().subdivisions(50);
    mSphere = gl::Batch::create(sphere, mGlsl);
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // setFullScreen(true);

    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 0, 5 });
    mCam.lookAt(vec3(0));
}

void SceneTest::update(double currentTime)
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
    ImGui::SliderFloat("Camera sensitivity", &mCam.mMouseSensitivity, 0.001, 0.3);
    ImGui::Separator();
    ImGui::Text("Key binding");
    ImGui::Text("W - Move forward");
    ImGui::Text("A - Move left");
    ImGui::Text("S - Move backward");
    ImGui::Text("D - Move right");
    ImGui::Text("Ctrl - Move downward");
    ImGui::Text("Space - Move upward");
    ImGui::Text("F - Freeze the camera");
    ImGui::Text("H - Toggle hidden cursor (for WSL)");
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

void SceneTest::draw()
{
    gl::clear(Color::gray(0.2f));
    gl::setMatrices(mCam);
    mSphere->draw();
}

Camera* SceneTest::getCamera()
{
    return &mCam;
}

void SceneTest::handleKeyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_f)
        mCam.toggleFreeze(mGlfwWindowRef);
    if (event.getCode() == KeyEvent::KEY_t)
        mCam.toggleFloating();
    if (event.getCode() == KeyEvent::KEY_h)
        mCam.toggleHiddenCursor(mGlfwWindowRef);

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

void SceneTest::handleMouseMove(MouseEvent event)
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

void SceneTest::processInput()
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