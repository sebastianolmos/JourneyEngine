#pragma once

#include "component.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Journey {
    class SkeletalModel;
    class Animator;

    class SkeletalMeshComponent : public Component {
        public:
            SkeletalMeshComponent() = default;

            std::shared_ptr<SkeletalModel> model;
            // For now, these meshes doesnt have lighting
            glm::vec3 color;

            virtual EComponentType GetType() override { return EComponentType::SkeletalMeshComponent; }

            std::shared_ptr<Animator> getAnimator();

    };

}