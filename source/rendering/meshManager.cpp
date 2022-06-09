#include "meshManager.hpp"
#include <glad/glad.h>

#include "../entity/entity.hpp"
#include "../component/staticMesh.hpp"
#include "material.hpp"
#include "model.hpp"
#include "importedModel.hpp"

namespace Journey
{
    void MeshManager::LoadPrimitiveMesh(std::string meshName, EPrimitiveMesh primitiveMesh, bool light)
    {
        std::size_t hashedName = mHasher(meshName);
        if (mMeshRecord.count(hashedName) != 0)
            return;

        switch(primitiveMesh) {
            case EPrimitiveMesh::Cube: {
                std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
                mMeshGenerator.GenerateCube(model, light);
                mMeshRecord.insert({hashedName, model});
                break;
                }
            case EPrimitiveMesh::Plane: {
                std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
                mMeshGenerator.GeneratePlane(model, light);
                mMeshRecord.insert({hashedName, model});
                break;
                }
            case EPrimitiveMesh::Pyramid: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Cylinder: {
                std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
                mMeshGenerator.GenerateCylinder(model, light, 16);
                mMeshRecord.insert({hashedName, model});
                break;
                }
            case EPrimitiveMesh::Sphere: {
                std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
                mMeshGenerator.GenerateSphere(model, light, 16);
                mMeshRecord.insert({hashedName, model});
                break;
                }
            case EPrimitiveMesh::Capsule: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            }
    }

    void MeshManager::LoadModelMesh(std::string meshName, std::string modelPath)
    {
        std::size_t hashedName = mHasher(meshName);
        if (mMeshRecord.count(hashedName) != 0)
            return;
        std::shared_ptr<ImportedModel> model = std::make_shared<ImportedModel>(modelPath);
        mMeshRecord.insert({hashedName, model});
    }

    void MeshManager::AddMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string meshName)
    {
        std::size_t hashedName = mHasher(meshName);
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) > 0)
            return;
        if (mMeshRecord.count(hashedName) == 0) {
            std::cout << meshName << " No registered"<< std::endl;
            return;
        }

        std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
        meshComp->material = mat; 
        meshComp->meshModel = mMeshRecord[hashedName];
        entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));

    }

}