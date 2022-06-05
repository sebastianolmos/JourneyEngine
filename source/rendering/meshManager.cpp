#include "meshManager.hpp"
#include <glad/glad.h>

#include "../entity/entity.hpp"
#include "../component/staticMesh.hpp"
#include "material.hpp"

namespace Journey
{

    void MeshManager::LoadPrimitiveMeshToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh)
    {
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) > 0)
            return;
        
        switch(primitiveMesh) {
            case EPrimitiveMesh::Cube: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::cout << "Material NOT Implemented yet" << std::endl;
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
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Sphere: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Capsule: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            }
    }

}