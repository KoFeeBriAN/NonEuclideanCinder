#include "Portal.h"

#include "cinder/Log.h"

double Portal::distance(Camera &cam) const 
{ 
    float el = 0.002f;
    auto pos = cam.getEyePoint();
    auto A = normal.x;
    auto B = normal.y;
    auto C = normal.z;
    auto D = -glm::dot(normal, origin);

    auto distance = (A * pos.x + B * pos.y + C * pos.z + D) / sqrt(glm::dot(normal, normal));
    CI_LOG_D(distance);

    return std::abs(distance);
}