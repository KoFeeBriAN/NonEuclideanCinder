#define GLFW_INCLUDE_NONE

#include "Resources.h"
#include "SceneTest.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "glfw/glfw3.h"
#include <unordered_map>

using namespace ci;
using namespace ci::app;

class MainApp : public App {
private:
    Scene* currentScene = new SceneTest();
    std::unordered_map<std::string, DataSourceRef> mSources;

public:
    void setup() override;
    void update() override;
    void draw() override;
    void mouseMove(MouseEvent event) override;
    void keyDown(KeyEvent event) override;

    static void prepareSettings(Settings* settings);
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
    mSources.insert({ "checkerboard.png", loadAsset("checkerboard.png") });
    currentScene->setWindow((GLFWwindow*)getWindow()->getNative());
    currentScene->setup(mSources);
}

void MainApp::update()
{
    currentScene->update(getElapsedSeconds());
}

void MainApp::draw()
{
    currentScene->draw();
}

CINDER_APP(MainApp, RendererGl(RendererGl::Options().msaa(16)), MainApp::prepareSettings)
