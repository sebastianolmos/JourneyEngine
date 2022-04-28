#pragma once
#include <memory>
#include <string>
#include <glad/glad.h>

namespace Journey {

    struct Texture
    {
        unsigned int textureId;
        int width;
        int height;
    };

    class Entity;
    struct Material;
    class TextureManager{
        public:

            TextureManager() = default;
            
            std::shared_ptr<Texture> LoadTexture(const std::string filePath, GLint magFilter, GLint minFilter,
                                                            GLint sWrapMode, GLint tWrapMode);

            void LoadSpriteToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string spritePath);
    };

}