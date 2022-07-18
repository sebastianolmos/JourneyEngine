#pragma once

#include "component.hpp"
#include "../rendering/material.hpp"
#include <memory>

namespace Journey {
    class SkeletalModel;
    class Animator;

    class SkeletalMeshComponent : public Component {
        public:
            SkeletalMeshComponent() = default;

            std::shared_ptr<SkeletalModel> model;
            std::shared_ptr<Material> material;

            virtual EComponentType GetType() override { return EComponentType::SkeletalMeshComponent; }

            std::shared_ptr<Animator> getAnimator();
            
            //virtual void UpdateRenderInfo(RenderInfo& rInfo) override {
            //    rInfo.modelObject = meshModel;
            //    material->fillRenderInfo(rInfo);
            //}
    };

}