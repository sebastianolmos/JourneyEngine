#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Journey {
    class Camera {

    public:
        virtual glm::mat4 getProjection() = 0;

        virtual glm::mat4 getViewMatrix() = 0;

        virtual glm::vec3 getViewPos() = 0;
    };
}