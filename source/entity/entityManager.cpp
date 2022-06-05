#include "entityManager.hpp"
#include "../scene/scene.hpp"
#include "../component/component.hpp"
#include "../component/staticMesh.hpp"
#include "../component/sprite.hpp"
#include "../rendering/material.hpp"

#include <iostream>
#include <unordered_map>
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
                rInfo.model = newTransform;
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
                rInfo.model = newTransform;
                scene.GetRenderManager().AddObjectToRender(sprite->material->GetType(), rInfo);
            }
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
        if (parentEntity == nullptr)
        {
            mEntities.insert({mEntitiesCount, newEntity});
        }
        else 
        {
            parentEntity->mChildren.insert({mEntitiesCount, newEntity});
        }
        mEntitiesCount++;
    }
}