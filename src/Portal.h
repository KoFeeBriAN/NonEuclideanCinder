#pragma once

#define GLFW_INCLUDE_NONE

#include "cinder/Buffer.h"
#include "cinder/gl/Batch.h"
#include "cinder/Camera.h"
#include "glm/glm.hpp"

using namespace ci;
using namespace glm;


typedef int PortalId;

class Portal {
public:
    PortalId id;
    vec3 normal;
    vec3 origin;
    vec2 size;

    gl::BatchRef batch;
public:
    double distance(Camera &cam) const;
};