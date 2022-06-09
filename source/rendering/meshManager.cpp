#include "meshManager.hpp"
#include <glad/glad.h>

#include "../entity/entity.hpp"
#include "../component/staticMesh.hpp"
#include "material.hpp"
#include "importedModel.hpp"

namespace Journey
{

    void MeshManager::AddPrimitiveMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh)
    {
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) > 0)
            return;
        
        switch(primitiveMesh) {
            case EPrimitiveMesh::Cube: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateCube(meshComp, false);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else if (mat->GetType() == EMaterialType::PhongColored || mat->GetType() == EMaterialType::FlatColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateCube(meshComp, true);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else {
                    std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
                }
                break;
                }
            case EPrimitiveMesh::Plane: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GeneratePlane(meshComp, false);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else if (mat->GetType() == EMaterialType::PhongColored || mat->GetType() == EMaterialType::FlatColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GeneratePlane(meshComp, true);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else {
                    std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
                }
                break;
                }
            case EPrimitiveMesh::Pyramid: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Cylinder: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateCylinder(meshComp, false, 16);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else if (mat->GetType() == EMaterialType::PhongColored || mat->GetType() == EMaterialType::FlatColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateCylinder(meshComp, true, 16);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else {
                    std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
                }
                break;
                }
            case EPrimitiveMesh::Sphere: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateSphere(meshComp, false, 16);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else if (mat->GetType() == EMaterialType::PhongColored || mat->GetType() == EMaterialType::FlatColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    mMeshGenerator.GenerateSphere(meshComp, true, 16);
                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else {
                    std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
                }
                break;
                }
            case EPrimitiveMesh::Capsule: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            }
    }

    void MeshManager::AddImportedMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string modelPath)
    {
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) > 0)
            return;

        if (mat->GetType() == EMaterialType::SimpleTextured) {
            std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
            std::shared_ptr<ImportedModel> model = std::make_shared<ImportedModel>(modelPath);

            meshComp->material = mat;
            meshComp->meshModel = model;
            entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
        } 
        else if (mat->GetType() == EMaterialType::PhongTextured || mat->GetType() == EMaterialType::FlatTextured) { 
            std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
            std::shared_ptr<ImportedModel> model = std::make_shared<ImportedModel>(modelPath);

            meshComp->material = mat;
            meshComp->meshModel = model;
            entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
        } 
        else {
            std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
        }
        
    }

}