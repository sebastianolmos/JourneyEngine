#pragma once

#include "material.hpp"

#include <vector>
#include <glm/glm.hpp>

#include "shaders/shader.h"

namespace Journey {

    struct RenderInfo{
        unsigned int VAO;
        unsigned int VBO;
        unsigned int textureID;
        unsigned int vertexCount;
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;   
        glm::fvec3 color;
        glm::mat4 model;    
    };

    class Scene;
    class RenderManager{
        public:
            RenderManager();

            void StartUp();
            void ShutDown();
            void CleanRenderInfo();

            void DrawCall(Scene& scene);

            void AddObjectToRender(EMaterialType materialType, RenderInfo renderInfo);

        private:	
            Shader SimpleColoredShader;
            Shader SimpleTexturedShader;
            Shader FlatColoredShader;
            Shader PhongColoredShader;
            Shader FlatTexturedShader;
            Shader PhongTexturedShader;

            std::vector<RenderInfo> mSimpleColoredObjects;
            std::vector<RenderInfo> mSimpleTexturedObjects;
            std::vector<RenderInfo> mFlatColoredObjects;
            std::vector<RenderInfo> mPhongColoredObjects;
            std::vector<RenderInfo> mFlatTexturedObjects;
            std::vector<RenderInfo> mPhongTexturedObjects;
    };

}