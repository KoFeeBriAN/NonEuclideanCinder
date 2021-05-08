#include "cinder/CinderImGui.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "glfw/glfw3.h"

#include "Resources.h"
#include "SceneTest.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MainApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void mouseMove(MouseEvent event) override;
    void keyDown(KeyEvent event) override;

    static void prepareSettings(Settings* settings);
    Scene* currentScene = new SceneTest();
};

void MainApp::prepareSettings(Settings* settings)
{
    settings->setResizable(true);
}

void MainApp::mouseMove(MouseEvent event)
{
    currentScene->handleMouseMove(event);
}

void MainApp::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE)
        quit();
    currentScene->handleKeyDown(event);
}

void MainApp::setup()
{
    currentScene->setWindow((GLFWwindow*)getWindow()->getNative());
    currentScene->setup();
}

void MainApp::update()
{
    currentScene->update();
}

void MainApp::draw()
{
    currentScene->draw();
}

CINDER_APP(MainApp, RendererGl(RendererGl::Options().msaa(16)), MainApp::prepareSettings)
