#include "cinder/CinderImGui.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"

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
}

void World::update()
{
    // vec3 pos = mCam.getEyePoint();
    // ImGui::Begin("CameraFP");
    // // ImGui::Text("Position: %f, %f, %f", pos.x, pos.y, pos.z);
    // ImGui::End();
}

void World::setup()
{
    gl::enableDepthWrite();
    gl::enableDepthRead();
    // hideCursor();
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
    gl::clear(Color(0.2f, 0.2f, 0.2f));
    gl::setMatrices(mCam);

    mSphere->draw();
}

CINDER_APP(World, RendererGl(RendererGl::Options().msaa(16)), World::prepareSettings)
