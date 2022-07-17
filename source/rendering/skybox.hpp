#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "shaders/shader.hpp"

namespace Journey {
    
    class Skybox {
    public:
        Skybox(std::vector<std::string> faces);
        void Draw(glm::mat4 view, glm::mat4 proj);

    private:
        unsigned int mVAO, mVBO;
        unsigned int mCubeMapTexture;
        Shader mShader;
    };

}