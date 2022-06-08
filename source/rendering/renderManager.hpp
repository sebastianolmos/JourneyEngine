#pragma once

#include "material.hpp"

#include <vector>
#include <queue>
#include <glm/glm.hpp>

#include "shaders/shader.h"

namespace Journey {

    struct RenderInfo{
        unsigned int VAO;
        unsigned int VBO;
        unsigned int textureID;
        unsigned int vertexCount;
        bool usingIndices = false;
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;   
        glm::fvec3 color;
        glm::mat4 model;    

        bool operator < (const RenderInfo& rInfo)const noexcept {
            return this->VAO < rInfo.VAO;
        }

        bool operator>(const RenderInfo& rInfo)const noexcept {
            return this->VAO > rInfo.VAO;
        }
    };
    
    typedef std::pair<float, RenderInfo> TransparentInfo;
    class Scene;

    class RenderManager{
        public:
            friend class Scene;
            RenderManager();

            void StartUp();
            void ShutDown();
            void CleanRenderInfo();

            void DrawCall(Scene& scene);

            void AddObjectToRender(EMaterialType materialType, RenderInfo renderInfo);
            void AddTransparentObjectToRender(float distanceToCam, RenderInfo renderInfo);

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
            std::priority_queue<TransparentInfo> mTransparentObjects;

            unsigned int mDrawLines = false;
            std::vector<RenderInfo> mDebugObjects;
            void CreateDebugObjects();
            void CreateDebugFrustrumObject();
            void CreateDebugAxisObject();
            void DrawDebugObjects(Shader shaderProgram, Scene& scene);
    };

}