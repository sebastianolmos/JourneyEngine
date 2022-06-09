#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "meshGenerator.hpp"
#include <unordered_map>

namespace Journey {

    enum class EPrimitiveMesh : uint8_t {
        Plane,
		Cube,
        Pyramid,
        Cylinder,
        Sphere,
        Capsule
	};
    class Entity;
    class Model;
    struct Material;

    class MeshManager{
        public:
            static MeshManager& getInstance() {
                static MeshManager instance;
                return instance;
            }
            MeshManager(MeshManager const&) = delete;    //Dont Implement!
            void operator=(MeshManager const&) = delete; //Dont Implement!

            void LoadPrimitiveMesh(std::string meshName, EPrimitiveMesh primitiveMesh, bool light = false);
            void LoadModelMesh(std::string meshName, std::string modelPath);
            void AddMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string meshName);

        private:
            MeshManager() {}

            MeshGenerator mMeshGenerator;

            std::hash<std::string> mHasher;
            std::unordered_map<std::size_t, std::shared_ptr<Model> > mMeshRecord;
    };
}