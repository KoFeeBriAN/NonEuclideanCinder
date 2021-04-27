#include "cinder/CinderImGui.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"
#include "glfw/glfw3.h"

#include "CameraFP.cpp"
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

    CameraFP mCam;
    gl::BatchRef mSphere;
    gl::TextureRef mTexture;
    gl::GlslProgRef mGlsl;

    // Camera attributes
    ivec2 lastPos;
    ivec2 currentPos;

    double mlastTime;
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

    double timeOffset = getElapsedSeconds() - mlastTime;
    if (event.getCode() == KeyEvent::KEY_w)
        mCam.move(MOVEMENT::FORWARD, timeOffset);
    if (event.getCode() == KeyEvent::KEY_s)
        mCam.move(MOVEMENT::BACKWARD, timeOffset);
    if (event.getCode() == KeyEvent::KEY_a)
        mCam.move(MOVEMENT::LEFT, timeOffset);
    if (event.getCode() == KeyEvent::KEY_d)
        mCam.move(MOVEMENT::RIGHT, timeOffset);
}

void World::update()
{
    // vec3 pos = mCam.getEyePoint();
    // ImGui::Begin("CameraFP");
    // // ImGui::Text("Position: %f, %f, %f", pos.x, pos.y, pos.z);
    // ImGui::End();
    mlastTime = getElapsedSeconds();
}

void World::setup()
{
    gl::enableDepthWrite();
    gl::enableDepthRead();

    glfwSetInputMode((GLFWwindow*)getWindow()->getNative(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // setFullScreen(true);

    mCam.setEyePoint({ 0, 0, 5 });
    mCam.lookAt(vec3(0));

    auto img = loadImage(loadAsset("checkerboard.png"));
    mTexture = gl::Texture::create(img);
    mTexture->bind();
    auto shader = gl::ShaderDef().texture().lambert();
    mGlsl = gl::getStockShader(shader);
    auto sphere = geom::Sphere().subdivisions(50);
    mSphere = gl::Batch::create(sphere, mGlsl);
}

void World::draw()
{
    gl::clear(Color::gray(0.3f));
    gl::setMatrices(mCam);

    mSphere->draw();
}

CINDER_APP(World, RendererGl(RendererGl::Options().msaa(16)), World::prepareSettings)
