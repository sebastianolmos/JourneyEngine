#include "skeletalManager.hpp"

#include "../entity/entity.hpp"
#include "../component/skeletalMesh.hpp"
#include "skeletalModel.hpp"

namespace Journey {

    void SkeletalManager::StartUp()
    {}

    void SkeletalManager::ShutDown()
    {
        cleanBatch();
    }

    std::shared_ptr<SkeletalMeshComponent> SkeletalManager::AddSkeletalMeshComponent(std::shared_ptr<Entity> entity, std::string path, glm::vec3 color)
    {
        std::shared_ptr<SkeletalModel> model = std::make_shared<SkeletalModel>(path);
        std::shared_ptr<SkeletalMeshComponent> meshComp = std::make_shared<SkeletalMeshComponent>();
        meshComp->color = color;
        meshComp->model = model;
        entity->mComponents.insert(std::make_pair(EComponentType::SkeletalMeshComponent, meshComp));
        return meshComp;
    }

    void SkeletalManager::cleanBatch()
    {
        mAnimatorsBatch.clear();
    }

    void SkeletalManager::updateAnimators(Scene& scene, float deltaTime)
    {
        for(auto& model: mAnimatorsBatch) {
            model->UpdateAnimator(deltaTime);
        }
    }

    void SkeletalManager::pushModelToBatch(std::shared_ptr<SkeletalModel> model)
    {
        mAnimatorsBatch.push_back(model);
    }

}