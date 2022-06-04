#pragma once

#include "camera.hpp"

namespace Journey {
    class DebugCamera : public Camera {
        
    public:

        DebugCamera();

        virtual glm::mat4 getProjection() override;

        virtual glm::mat4 getViewMatrix() override;

        virtual glm::vec3 getViewPos() override;

        unsigned int SCR_WIDTH;
        unsigned int SCR_HEIGHT;

        float x = 0.0;
        float y = -0.5;
        float z = 1.2;
        glm::vec3 camDir = glm::vec3(0.0f, 0.6, -0.3);

    };
}