#pragma once
#include "cinder/Camera.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/app/MouseEvent.h"
#include "glfw/glfw3.h"

using namespace ci::app;
using namespace ci;

class Scene {
public:
    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual Camera* getCamera() = 0;
    virtual void handleMouseMove(MouseEvent event) = 0;
    virtual void handleKeyDown(KeyEvent event) = 0;
    void setWindow(GLFWwindow* window);

protected:
    GLFWwindow* mGlfwWindowRef;
};