#pragma once
#include <memory>
#include <string>

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
            
            std::shared_ptr<Texture> LoadTexture(const std::string filePath, int magFilter, int minFilter,
                                                            int sWrapMode, int tWrapMode);

            void LoadSpriteToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string spritePath);
    };

}