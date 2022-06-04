#pragma once

#include <cstdint>
#include <memory>
#include <string>

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

            MeshManager() = default;

            void LoadPrimitiveMeshToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh);
    };
}