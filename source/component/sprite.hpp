#pragma once

#include "component.hpp"

namespace Journey {
    struct Material;
    class SpriteComponent : public Component {
        public:
            SpriteComponent() = default;

            float* vertices;
            int vertexCount;
            int vertexStride;

            unsigned int VAO;
            unsigned int VBO;
            unsigned textureID;

            std::shared_ptr<Material> material;
    };

}