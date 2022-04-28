#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Journey{

    class CameraHandler {
        public:
            
            CameraHandler() = default;

            const unsigned int SCR_WIDTH = 800;
            const unsigned int SCR_HEIGHT = 600;

            float x = 0.0;
            float y = -0.5;
            float z = 1.2;
            glm::vec3 camDir = glm::vec3(0.0f, 0.6, -0.3);
            
            glm::mat4 getProjection() {
                return glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            }

            glm::mat4 getViewMatrix() {
                return glm::lookAt(glm::vec3(x, y , z) - camDir * 2.0f, glm::vec3(x, y , z), glm::vec3(0.0, 0.0, 1.0f));
            }

            glm::vec3 getViewPos(){
                return  glm::vec3(x, y , z) - camDir * 2.0f;
            }
    };

}