#include "Portal.h"

#include "cinder/Log.h"
#include <string>

double Portal::distance(vec3 position) const
{
    float el = 0.002f;
    auto A = normal.x;
    auto B = normal.y;
    auto C = normal.z;
    auto D = -glm::dot(normal, origin);

    auto distance = (A * position.x + B * position.y + C * position.z + D) / sqrt(glm::dot(normal, normal));

    return std::abs(distance);
}

bool Portal::inside(vec3 position) const
{
    auto left = origin.z - size.y / 2;
    auto right = origin.z + size.y / 2;
    auto top = origin.y + size.x / 2;
    auto bottom = origin.y - size.x / 2;

    // std::string str = "";
    // str += "left: " + std::to_string(left) + " ";
    // str += "right: " + std::to_string(right) + " ";
    // str += "top: " + std::to_string(top) + " ";
    // str += "bottom: " + std::to_string(bottom) + " \n";
    // str += "pos z: " + std::to_string(position.z) + " ";
    // str += "pos y: " + std::to_string(position.y) + " ";

    // CI_LOG_D(str);

    return position.z > left && position.z < right && position.y > bottom && position.y < top;
}