#include "Scene.h"

Scene::~Scene() { }

void Scene::setWindow(GLFWwindow* window)
{
    mGlfwWindowRef = window;
}