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
    EntityManager::EntityManager(){
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
            
            std::cout << "DebuG 10" << std::endl;
            StaticMeshComponent* staticMesh = dynamic_cast<StaticMeshComponent*>((entity->mComponents[EComponentType::StaticMeshComponent]).get());
            if (staticMesh != nullptr)
            {   
                std::cout << "DebuG 9" << std::endl;
                RenderInfo rInfo;
                std::cout << "DebuG 101" << std::endl;
                rInfo.VAO = staticMesh->VAO;
                std::cout << "DebuG 102" << std::endl;
                rInfo.VBO = staticMesh->VBO;
                std::cout << "DebuG 103" << std::endl;
                rInfo.faces = staticMesh->vertexCount;
                std::cout << "DebuG 104" << std::endl;
                switch(staticMesh->material->GetType()) {
                    case EMaterialType::SimpleColored:
                        std::cout << "DebuG 105" << std::endl;
                        rInfo.ks = dynamic_cast<SimpleColoredMaterial*>((staticMesh->material).get())->color;
                        break;
                    case EMaterialType::SimpleTextured:
                        break;
                    case EMaterialType::FlatColored:
                        rInfo.ks = dynamic_cast<FlatColoredMaterial*>((staticMesh->material).get())->ks;
                        rInfo.kd = dynamic_cast<FlatColoredMaterial*>((staticMesh->material).get())->kd;
                        rInfo.ke = dynamic_cast<FlatColoredMaterial*>((staticMesh->material).get())->ke;
                        break;
                    case EMaterialType::PhongColored:
                        rInfo.ks = dynamic_cast<PhongColoredMaterial*>((staticMesh->material).get())->ks;
                        rInfo.kd = dynamic_cast<PhongColoredMaterial*>((staticMesh->material).get())->kd;
                        rInfo.ke = dynamic_cast<PhongColoredMaterial*>((staticMesh->material).get())->ke;
                        break;
                    case EMaterialType::FlatTextured:
                        rInfo.ks = dynamic_cast<FlatTexturedMaterial*>(staticMesh->material.get())->ks;
                        rInfo.kd = dynamic_cast<FlatTexturedMaterial*>(staticMesh->material.get())->kd;
                        rInfo.ke = dynamic_cast<FlatTexturedMaterial*>(staticMesh->material.get())->ke;
                        break;
                    case EMaterialType::PhongTextured:
                        rInfo.ks = dynamic_cast<PhongTexturedMaterial*>(staticMesh->material.get())->ks;
                        rInfo.kd = dynamic_cast<PhongTexturedMaterial*>(staticMesh->material.get())->kd;
                        rInfo.ke = dynamic_cast<PhongTexturedMaterial*>(staticMesh->material.get())->ke;
                        break;
                    default:
                        // code block
                        break;
                }               
                rInfo.model = newTransform;
                std::cout << "DebuG 7" << std::endl;
                scene.GetRenderManager().AddObjectToRender(staticMesh->material->GetType(), rInfo);
            }
        }
        else if (((entity->mComponents).count(EComponentType::SpriteComponent) != 0 ))
        {
            SpriteComponent* sprite = dynamic_cast<SpriteComponent*>((entity->mComponents[EComponentType::SpriteComponent]).get());
            if (sprite != nullptr)
            {   
                std::cout << "DebuG 100" << std::endl;
                RenderInfo rInfo;
                rInfo.VAO = sprite->VAO;
                rInfo.VBO = sprite->VBO;
                rInfo.faces = sprite->vertexCount;
                switch(sprite->material->GetType()) {
                    case EMaterialType::SimpleColored:
                        rInfo.ks = dynamic_cast<SimpleColoredMaterial*>((sprite->material).get())->color;
                        break;
                    case EMaterialType::SimpleTextured:
                        rInfo.textureID = sprite->textureID;
                        break;
                    case EMaterialType::FlatColored:
                        rInfo.ks = dynamic_cast<FlatColoredMaterial*>(sprite->material.get())->ks;
                        rInfo.kd = dynamic_cast<FlatColoredMaterial*>(sprite->material.get())->kd;
                        rInfo.ke = dynamic_cast<FlatColoredMaterial*>(sprite->material.get())->ke;
                        break;
                    case EMaterialType::PhongColored:
                        rInfo.ks = dynamic_cast<PhongColoredMaterial*>(sprite->material.get())->ks;
                        rInfo.kd = dynamic_cast<PhongColoredMaterial*>(sprite->material.get())->kd;
                        rInfo.ke = dynamic_cast<PhongColoredMaterial*>(sprite->material.get())->ke;
                        break;
                    case EMaterialType::FlatTextured:
                        rInfo.ks = dynamic_cast<FlatTexturedMaterial*>(sprite->material.get())->ks;
                        rInfo.kd = dynamic_cast<FlatTexturedMaterial*>(sprite->material.get())->kd;
                        rInfo.ke = dynamic_cast<FlatTexturedMaterial*>(sprite->material.get())->ke;
                        break;
                    case EMaterialType::PhongTextured:
                        rInfo.ks = dynamic_cast<PhongTexturedMaterial*>(sprite->material.get())->ks;
                        rInfo.kd = dynamic_cast<PhongTexturedMaterial*>(sprite->material.get())->kd;
                        rInfo.ke = dynamic_cast<PhongTexturedMaterial*>(sprite->material.get())->ke;
                        break;
                    default:
                        // code block
                        break;
                }               
                rInfo.model = newTransform;
                std::cout << "DebuG 8" << std::endl;
                scene.GetRenderManager().AddObjectToRender(sprite->material->GetType(), rInfo);
            }
        }

        for (std::pair<int, std::shared_ptr<Entity>> element : entity->mChildren)
        {
            UpdateEntity(scene, element.second, newTransform, deltaTime);
        }
        std::cout << "DebuG 3" << deltaTime << std::endl;
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