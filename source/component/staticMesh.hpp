#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    class StaticMeshComponent : public Component {
        public:
            StaticMeshComponent() = default;

            float* vertices;
            int vertexCount;
            int vertexStride;

            unsigned int VAO;
            unsigned int VBO;
            unsigned textureID;
            bool usingIndicesToRender = false;

            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::StaticMeshComponent; }

            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.VAO = VAO;
                rInfo.VBO = VBO;
                rInfo.vertexCount = vertexCount;
                rInfo.usingIndices = usingIndicesToRender;
                material->fillRenderInfo(rInfo);
            }
    };

}