#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    struct Material;
    class SpriteComponent : public Component {
        public:
            SpriteComponent() = default;

            bool transparency;
            std::shared_ptr<SpriteModel> spriteModel;
            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::SpriteComponent; }
            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.modelObject = spriteModel;
                material->fillRenderInfo(rInfo);        
            }
    };

}