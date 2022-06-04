#include "debugCamera.hpp"

namespace Journey {

    DebugCamera::DebugCamera()
    {}

    glm::mat4 DebugCamera::getProjection()
    {
        return glm::perspective(glm::radians(90.0f), (float)1600 / (float)900, 0.1f, 100.0f);
    }

    glm::mat4 DebugCamera::getViewMatrix()
    {
        return glm::lookAt(glm::vec3(x, y , z) - camDir * 2.0f, glm::vec3(x, y , z), glm::vec3(0.0, 0.0, 1.0f));

    }

    glm::vec3 DebugCamera::getViewPos()
    {
        return  glm::vec3(x, y, z) - camDir * 2.0f;
    }
}