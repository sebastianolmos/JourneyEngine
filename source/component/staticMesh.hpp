#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"

namespace Journey {
    class StaticMeshComponent : public Component {
        public:
            StaticMeshComponent() = default;

            std::shared_ptr<StaticMeshModel> meshModel;
            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::StaticMeshComponent; }

            
            virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
                rInfo.modelObject = meshModel;
                material->fillRenderInfo(rInfo);
            }
    };

}