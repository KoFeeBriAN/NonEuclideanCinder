#include "SceneTunnel.h"

#include "cinder/CinderImGui.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/wrapper.h"

using namespace ci;

void SceneTunnel::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    // set GLFW
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // prepare texture and glsl program
    auto img = loadImage(assets.at("checkerboard.png"));
    mTexture = gl::Texture::create(img);
    mTexture->bind();
    mTexture->setWrap(GL_REPEAT, GL_REPEAT);
    auto shader = gl::ShaderDef().texture();
    mGlsl = gl::getStockShader(shader);

    // Main sphere
    // auto sphere = gl::Batch::create(geom::Sphere().subdivisions(50), mGlsl);
    // mBatches.push_back(sphere);
    // Ground plane
    auto floor = gl::Batch::create(geom::Plane().size({ 69, 69 }), mGlsl);
    mBatches.push_back(floor);

    // Tunnel
    auto m1 = geom::Cube().size({ 1, 13, 10 }) >> geom::Transform(translate(vec3(-10, 0, 0)));
    mBatches.push_back(gl::Batch::create(m1, mGlsl));
    m1 = m1 >> geom::Transform(translate(vec3(0, 20, 0))) >> geom::Transform(rotate(180.0f, vec3(0, 0, 1)));
    mBatches.push_back(gl::Batch::create(m1, mGlsl));

    // setFullScreen(true);

    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 3, 3 });
    mCam.lookAt({ 0, 1, 3 });
    mCam.mFloating = false;
}

void SceneTunnel::update(double currentTime)
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
    ImGui::SliderFloat("Cube X", &cubex, 1, 10);
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

    // Update cube
    mCube.size({ cubex, 13, 10 });
    // mBatches[1] = gl::Batch::create(mCube, mGlsl);

    // Poll for inputs
    processInput();
}

void SceneTunnel::draw()
{
    gl::clear(Color::gray(0.2f));
    gl::setMatrices(mCam);

    for (auto model : mBatches)
        model->draw();
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
