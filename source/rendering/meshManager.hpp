#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "meshGenerator.hpp"

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
    struct Material;

    class MeshManager{
        public:
            static MeshManager& getInstance() {
                static MeshManager instance;
                return instance;
            }
            MeshManager(MeshManager const&) = delete;    //Dont Implement!
            void operator=(MeshManager const&) = delete; //Dont Implement!

            void AddPrimitiveMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh);

        private:
            MeshManager() {}

            MeshGenerator mMeshGenerator;

    };
}