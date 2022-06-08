#pragma once
#include <memory>
#include <string>

namespace Journey {

    struct TextureRep
    {
        unsigned int textureId;
        int width;
        int height;
    };
    class Entity;
    struct Material;

    class TextureManager{
        public:
            static TextureManager& getInstance() {
                static TextureManager instance;
                return instance;
            }
            TextureManager(TextureManager const&) = delete;
            void operator=(TextureManager const&) = delete;
            
            std::shared_ptr<TextureRep> LoadTexture(const std::string filePath, int magFilter, int minFilter,
                                                            int sWrapMode, int tWrapMode);

            void AddSpriteComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string spritePath, bool transparency=false);

        private:
            TextureManager() {}

    };

}