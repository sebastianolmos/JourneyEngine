#pragma once

#include "material.hpp"

#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <memory>

#include "shaders/shader.hpp"
#include "model.hpp"
#include "importedModel.hpp"

namespace Journey {
    class PointLightComponent;
    class SpotLightComponent;
    class Skybox;
    class BloomRenderer;

    struct RenderInfo{
        std::shared_ptr<Model> modelObject;
        glm::fvec3 ks;
        glm::fvec3 kd;
        glm::fvec3 ke;   
        glm::fvec3 color;
        glm::mat4 modelTransform;    

        bool operator < (const RenderInfo& rInfo)const noexcept {
            return this->ks.x < rInfo.ks.x;
        }

        bool operator>(const RenderInfo& rInfo)const noexcept {
            return this->ks.x> rInfo.ks.x;
        }
    };

    struct RenderDebugInfo{
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
    };

    struct SpotLightInfo {
        glm::vec3 position;
        glm::vec3 direction;
        SpotLightComponent* light;
    };

    typedef std::pair<float, RenderInfo> TransparentInfo;
    typedef std::pair<glm::vec3, PointLightComponent* > PointLightInfo;
    class Scene;

    class RenderManager{
        public:
            friend class Scene;
            RenderManager();

            void StartUp(glm::ivec2 dim);
            void ShutDown();
            void CleanRenderInfo();

            void DrawCall(Scene& scene);

            void AddObjectToRender(EMaterialType materialType, RenderInfo renderInfo);
            void AddTransparentObjectToRender(float distanceToCam, RenderInfo renderInfo);
            void AddPointLightToRender(glm::vec3 pos, PointLightComponent* light);
            void AddSpotLightToRender(glm::vec3 pos, glm::vec3 dir, SpotLightComponent* light);
            void AddSkyBox(std::vector<std::string> faces);

            bool isHDREnabled();
            bool isBloomEnabled();
            float getGammaValue();
            float getExposureValue();
            float getBloomFilterRadius();
            float getBloomStrength();
            void setHDREnabled(bool value);
            void setBloomEnabled(bool value);
            void setGammaValue(float value);
            void setExposureValue(float value);
            void setBloomFilterRadius(float value);
            void setBloomStrength(float value);

        private:	
            Shader SimpleColoredShader;
            Shader SimpleTexturedShader;
            Shader FlatColoredShader;
            Shader PhongColoredShader;
            Shader FlatTexturedShader;
            Shader PhongTexturedShader;
            Shader bloomFinalShader;

            std::vector<RenderInfo> mSimpleColoredObjects;
            std::vector<RenderInfo> mSimpleTexturedObjects;
            std::vector<RenderInfo> mFlatColoredObjects;
            std::vector<RenderInfo> mPhongColoredObjects;
            std::vector<RenderInfo> mFlatTexturedObjects;
            std::vector<RenderInfo> mPhongTexturedObjects;
            std::priority_queue<TransparentInfo> mTransparentObjects;

            unsigned int mDrawLines = false;
            std::vector<RenderDebugInfo> mDebugObjects;
            
            const unsigned int MaxPointLights = 16;
            int currentPointLights = 0;
            std::vector<PointLightInfo> mPointLights;

            const unsigned int MaxSpotLights = 16;
            int currentSpotLights = 0;
            std::vector<SpotLightInfo> mSpotLights;

            Skybox* mSkybox = nullptr;

            unsigned int mHdrFBO;
            unsigned int mColorBuffers[2];
            unsigned int mRboDepth;
            unsigned int mScreenWidth, mScreenHeight;
            BloomRenderer* mBloomRenderer;
            bool mHDR = true;
            bool mBloom = true;
            float mGamma = 2.0;
            float mExposure = 1.2f;
            float mBloomFilterRadius = 0.009f;
            float mBloomStrength = 0.108f;

            void ConfigureFloatingPointFrameBuffer();
            void CreateAndAttachDepthBuffer();
            void CreateDebugObjects();
            void CreateDebugFrustrumObject();
            void CreateDebugAxisObject();
            void DrawDebugObjects(Shader shaderProgram, Scene& scene);
    };

}