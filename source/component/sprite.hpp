#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    struct Material;
    class SpriteComponent : public Component {
        public:
            SpriteComponent() = default;

            float* vertices;
            int vertexCount;
            int vertexStride;

            unsigned int VAO;
            unsigned int VBO;
            unsigned textureID;

            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::SpriteComponent; }
            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.VAO = VAO;
                rInfo.VBO = VBO;
                rInfo.vertexCount = vertexCount;
                if (material->GetType() == EMaterialType::SimpleTextured) 
                    rInfo.textureID = textureID;
                material->fillRenderInfo(rInfo);        
            }
    };

}