#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <memory>
#include <string>

namespace Journey {

    enum class EPrimitiveMesh : uint8_t {
		Cube,
        Sphere
	};
    
    class Entity;
    struct Material;
    class MeshManager{
        public:

            MeshManager() = default;

            void LoadPrimitiveMeshToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh);
    };
}