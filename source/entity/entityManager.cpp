#include "entityManager.hpp"
#include "../scene/scene.hpp"
#include "../component/component.hpp"
#include "../component/staticMesh.hpp"
#include "../component/audioSource.hpp"
#include "../component/pointLight.hpp"
#include "../component/spotLight.hpp"
#include "../component/sprite.hpp"
#include "../component/skeletalMesh.hpp"
#include "../rendering/material.hpp"
#include "../audio/audioManager.hpp"
#include "../skeletal/skeletalManager.hpp"
#include  <glm/gtx/norm.hpp>

#include <iostream>
#include <unordered_map>
#include <utility>   
#include <memory>


namespace Journey {
    EntityManager::EntityManager(Scene& scene)
    :mSceneRef(scene) 
    {
        mEntitiesCount = 0;
    }

    void EntityManager::StartUp() 
    {}

    void EntityManager::ShutDown()
    {}

    void EntityManager::UpdateEntity(Scene& scene, std::shared_ptr<Entity> entity, glm::mat4 parentTransform, float deltaTime)
    {
        entity->Update(scene, deltaTime);

        glm::mat4 newTransform = parentTransform * (entity->getTransform().GetModelMatrix());

        //Check StaticMesh Component
        if ((entity->mComponents).count(EComponentType::StaticMeshComponent) != 0 )
        {
            StaticMeshComponent* staticMesh = dynamic_cast<StaticMeshComponent*>((entity->mComponents[EComponentType::StaticMeshComponent]).get());
            if (staticMesh != nullptr)
            {   
                RenderInfo rInfo;
                staticMesh->UpdateRenderInfo(rInfo);
                rInfo.modelTransform = newTransform;
                scene.GetRenderManager().AddObjectToRender(staticMesh->material->GetType(), rInfo);
            }
        }
        else if (((entity->mComponents).count(EComponentType::SpriteComponent) != 0 ))
        {
            SpriteComponent* sprite = dynamic_cast<SpriteComponent*>((entity->mComponents[EComponentType::SpriteComponent]).get());
            if (sprite != nullptr)
            {   
                RenderInfo rInfo;
                sprite->UpdateRenderInfo(rInfo);
                rInfo.modelTransform = newTransform;
                if (sprite->transparency) {
                    glm::vec3 position = glm::vec3(newTransform[3]);
                    float dist2 = glm::distance2(position, scene.GetCameraHandler().getViewPos());
                    scene.GetRenderManager().AddTransparentObjectToRender(dist2, rInfo);
                }
                else
                    scene.GetRenderManager().AddObjectToRender(sprite->material->GetType(), rInfo);
            }
        }
        else if (((entity->mComponents).count(EComponentType::SkeletalMeshComponent) != 0 ))
        {
            SkeletalMeshComponent* skelMesh = dynamic_cast<SkeletalMeshComponent*>((entity->mComponents[EComponentType::SkeletalMeshComponent]).get());
            scene.GetSkeletalManager().pushModelToBatch(skelMesh->model);
            scene.GetRenderManager().AddSkeletalObjectToRender(skelMesh->color, newTransform, skelMesh->model);
        }

        //Check AudioSource Component
        if ((entity->mComponents).count(EComponentType::AudioSourceComponent) != 0 )
        {
            AudioSourceComponent* audioSrc = dynamic_cast<AudioSourceComponent*>((entity->mComponents[EComponentType::AudioSourceComponent]).get());
            scene.GetAudioManager().pushSourceToBatch(newTransform[3], audioSrc->getBuffer());
        }

        // Check Ligths Component
        if ((entity->mComponents).count(EComponentType::PointLightComponent) != 0 )
        {
            PointLightComponent* light = dynamic_cast<PointLightComponent*>((entity->mComponents[EComponentType::PointLightComponent]).get());
            scene.GetRenderManager().AddPointLightToRender(newTransform[3], light);
        }
        else if ((entity->mComponents).count(EComponentType::SpotLightComponent) != 0 )
        {
            SpotLightComponent* light = dynamic_cast<SpotLightComponent*>((entity->mComponents[EComponentType::SpotLightComponent]).get());
            glm::vec4 trDirection = entity->getTransform().GetLocalRotation() * glm::vec4(light->getDirection(), 1.0f);
            scene.GetRenderManager().AddSpotLightToRender(newTransform[3], glm::normalize(glm::vec3(trDirection.x, trDirection.y, trDirection.z)/trDirection.w), light);
        }

        for (std::pair<int, std::shared_ptr<Entity>> element : entity->mChildren)
        {
            UpdateEntity(scene, element.second, newTransform, deltaTime);
        }
    }

    void EntityManager::UpdateEntities(Scene& scene, float deltaTime)
    {
        for (std::pair<int, std::shared_ptr<Entity>> element : mEntities)
        {
            UpdateEntity(scene, element.second, glm::mat4(1.0f), deltaTime);
        }
    }

    void EntityManager::AddEntity(std::shared_ptr<Entity> parentEntity, std::shared_ptr<Entity> newEntity)
    {   
        newEntity->StartUp(mSceneRef, shared_from_this());
        newEntity->mId = mEntitiesCount;
        if (parentEntity == nullptr)
        {
            mEntities.insert({mEntitiesCount, newEntity});
        }
        else 
        {   
            newEntity->mParent = parentEntity;
            parentEntity->mChildren.insert({mEntitiesCount, newEntity});
        }
        mEntitiesCount++;
    }

    void EntityManager::DeleteEntity(std::shared_ptr<Entity> entity) 
    {
        if (entity->mCurrentState != EState::PendingDestroy) {
            entity->mCurrentState = EState::PendingDestroy;
            mDeletedEntities.push_back(entity);

            if (entity->HasComponent(EComponentType::PointLightComponent))
                mSceneRef.deletePointLight();

            if (entity->HasComponent(EComponentType::SpotLightComponent))
                mSceneRef.deleteSpotLight();
        }
        
    }

    void EntityManager::DeleteEntities()
    {   
        for(auto& entity: mDeletedEntities) {
            if (entity->mParent == nullptr) {
                if (mEntities.count(entity->mId) > 0){
                    mEntities.erase(entity->mId);
                    entity->ShutDown();
                }
            }
            else {
                if (entity->mParent->mChildren.count(entity->mId) > 0) {
                    entity->mParent->mChildren.erase(entity->mId);
                    entity->ShutDown();
                }
            }
        }
        mDeletedEntities.clear();
    }
}