#pragma once 

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace Journey {
    class Entity;
    class Scene;
    class SkeletalMeshComponent;
    class Animator;
    class SkeletalModel;

    class SkeletalManager {
    public:
        friend class Scene;
        SkeletalManager() {};

        void StartUp();
        void ShutDown();

        std::shared_ptr<SkeletalMeshComponent> AddSkeletalMeshComponent(std::shared_ptr<Entity> entity, std::string path, glm::vec3 color);
        void pushModelToBatch(std::shared_ptr<SkeletalModel> model);
    private:
        void cleanBatch();
        void updateAnimators(Scene& scene, float deltaTime);
        std::vector< std::shared_ptr<SkeletalModel> > mAnimatorsBatch;
    };

}