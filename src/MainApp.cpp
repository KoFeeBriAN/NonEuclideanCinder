#define GLFW_INCLUDE_NONE

#include "Resources.h"
#include "SceneTest.h"
#include "SceneTest2.h"
#include "SceneTunnel.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "glfw/glfw3.h"
#include <unordered_map>

using namespace ci;
using namespace ci::app;

enum class SCENE {
    TEST,
    TEST2,
    TUNNEL
};

class MainApp : public App {
private:
    Scene* currentScene = new SceneTest();
    std::unordered_map<std::string, DataSourceRef> mSources;
    int mWindowWidth = 1000;
    int mWindowHeight = 800;

public:
    void setup() override;
    void update() override;
    void draw() override;
    void mouseMove(MouseEvent event) override;
    void keyDown(KeyEvent event) override;
    void resize() override;
    static void prepareSettings(Settings* settings);

private:
    void switchScene(SCENE scene);
    void toggleFullscreen();
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
    if (event.getCode() == KeyEvent::KEY_g)
        toggleFullscreen();

    if (event.getCode() == KeyEvent::KEY_1)
        switchScene(SCENE::TEST);
    if (event.getCode() == KeyEvent::KEY_2)
        switchScene(SCENE::TEST2);
    if (event.getCode() == KeyEvent::KEY_3)
        switchScene(SCENE::TUNNEL);

    currentScene->handleKeyDown(event);
}

void MainApp::resize()
{
    mWindowWidth = getWindowWidth();
    mWindowHeight = getWindowHeight();
    currentScene->getCamera()->setAspectRatio(getWindowAspectRatio());
}

void MainApp::toggleFullscreen()
{
    if (isFullScreen()) {
        setWindowSize(mWindowWidth, mWindowHeight);
        setFullScreen(false);
    } else {
        setFullScreen(true);
    }
}

void MainApp::setup()
{
    mSources.insert({ "checkerboard.png", loadAsset("checkerboard.png") });
    mSources.insert({ "rock-tunnel", loadAsset("cartoon-rock-texture.jpg") });
    setWindowSize(mWindowWidth, mWindowHeight);
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

void MainApp::switchScene(SCENE scene)
{
    if (currentScene != nullptr)
        delete currentScene;

    switch (scene) {
    case SCENE::TEST2:
        currentScene = new SceneTest2();
        break;

    case SCENE::TUNNEL:
        currentScene = new SceneTunnel();
        break;

    default:
        currentScene = new SceneTest();
        break;
    }
    currentScene->setWindow((GLFWwindow*)getWindow()->getNative());
    currentScene->setup(mSources);
}

CINDER_APP(MainApp, RendererGl(RendererGl::Options().msaa(16)), MainApp::prepareSettings)
