#include "SceneRoom.h"

#include "cinder/CinderImGui.h"
#include "cinder/gl/Shader.h"
#include "cinder/gl/wrapper.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

using namespace ci;

void SceneRoom::setup(const std::unordered_map<std::string, DataSourceRef>& assets)
{
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Initialize ImGui
    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 0, 5 });                  // set camera position
    mCam.lookAt(vec3(0., -5., -5.));                // set view direction

    // Setup Plane
    auto img_checkerboard = loadImage(assets.at("checkerboard.png"));
    auto fmt = gl::Texture::Format(); 
    fmt.setWrap( GL_REPEAT, GL_REPEAT );
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    mPlaneTexture = gl::Texture::create(img_checkerboard, fmt);
    mPlaneTexture->bind();

    auto shader = gl::ShaderDef().texture( mPlaneTexture ).lambert();
    mPlaneShader = gl::getStockShader(shader);

    auto plane = geom::Plane()
        .size(glm::vec2(1200., 1200.))
        .origin(glm::vec3(0., -5., -5.));

    mPlane = gl::Batch::create(plane, mPlaneShader);

    // Setup Wall
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
    gl::setMatrices(mCam);                  // set matrix scene to match the camera

    // Enable depth buffer
    gl::ScopedDepth depth( true );

    // Draw Plane Floor
    mPlane->draw();
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