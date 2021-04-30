#include "cinder/CinderImGui.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"
#include "glfw/glfw3.h"

#include "CameraFP.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class World : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void mouseMove(MouseEvent event) override;
    void keyDown(KeyEvent event) override;

    static void prepareSettings(Settings* settings);
    void processInput();

    CameraFP mCam;
    gl::BatchRef mSphere;
    gl::TextureRef mTexture;
    gl::GlslProgRef mGlsl;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
    double mTimeOffset;
    GLFWwindow* mGlfwWindowRef = (GLFWwindow*)getWindow()->getNative();
};

void World::prepareSettings(Settings* settings)
{
    settings->setResizable(true);
}

void World::mouseMove(MouseEvent event)
{
    static bool firstMouseMove = true;

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

void World::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE)
        quit();
    if (event.getCode() == KeyEvent::KEY_f)
        mCam.toggleFreeze(mGlfwWindowRef);
}

void World::setup()
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    glfwSetInputMode(mGlfwWindowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // setFullScreen(true);

    ImGui::Initialize();

    // initialize camera properties
    mCam.setEyePoint({ 0, 0, 5 });
    mCam.lookAt(vec3(0));

    // prepare batch program
    auto img = loadImage(loadAsset("checkerboard.png"));
    mTexture = gl::Texture::create(img);
    mTexture->bind();
    auto shader = gl::ShaderDef().texture().lambert();
    mGlsl = gl::getStockShader(shader);
    auto sphere = geom::Sphere().subdivisions(50);
    mSphere = gl::Batch::create(sphere, mGlsl);
}

void World::update()
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
    ImGui::Text("Key binding");
    ImGui::Text("W - Move forward");
    ImGui::Text("A - Move left");
    ImGui::Text("S - Move backward");
    ImGui::Text("D - Move right");
    ImGui::Text("Ctrl - Move downward");
    ImGui::Text("Space - Move upward");
    ImGui::Text("F - Freeze the camera");
    ImGui::Text("Esc - Close applicaiton");
    ImGui::End();

    // Update time logic
    double currentTime = getElapsedSeconds();
    mTimeOffset = currentTime - mlastTime;
    mlastTime = currentTime;

    // Poll for inputs
    processInput();
}

void World::processInput()
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

void World::draw()
{
    gl::clear(Color::gray(0.3f));
    gl::setMatrices(mCam);

    mSphere->draw();
}

CINDER_APP(World, RendererGl(RendererGl::Options().msaa(16)), World::prepareSettings)
