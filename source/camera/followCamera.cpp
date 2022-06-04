#include "followCamera.hpp"

namespace Journey {

    FollowCamera::FollowCamera(const unsigned int screenWidth, const unsigned int screenHeight)
    {
        SCR_WIDTH = screenWidth;
        SCR_HEIGHT = screenHeight;
    }

    glm::mat4 FollowCamera::getProjection()
    {
        return glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 30.0f);
    }

    glm::mat4 FollowCamera::getViewMatrix()
    {
        return glm::lookAt(glm::vec3(x, y , z) - camDir * 2.0f, glm::vec3(x, y , z), glm::vec3(0.0, 0.0, 1.0f));

    }

    glm::vec3 FollowCamera::getViewPos()
    {
        return  glm::vec3(x, y, z) - camDir * 2.0f;
    }
}