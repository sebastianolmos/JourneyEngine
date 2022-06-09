#pragma once

#include <glm/glm.hpp>

namespace Journey{

    class PointLight {
        public: 
            PointLight() = default;

            glm::vec3 lightPos = glm::vec3(1.5f, 1.0f, 5.5f);
            glm::vec3 diffuseColor = glm::vec3(1.0f); // decrease the influence
            glm::vec3 ambientColor = glm::vec3(1.0f);
            glm::vec3 specularColor = glm::vec3(1.0f);

            glm::vec3 getLightPos() {
                return lightPos;
            }

            glm::vec3 getDifuseColor() {
                return diffuseColor;
            }

            glm::vec3 getAmbientColor() {
                return ambientColor;
            }

            glm::vec3 getSpecularColor() {
                return specularColor;
            }

            void setLightPos(glm::vec3 newPos) {lightPos = newPos;}
    };

}