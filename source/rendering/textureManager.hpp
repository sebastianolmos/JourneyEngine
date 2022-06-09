#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Journey {

    struct TextureRep
    {
        unsigned int textureId;
        int width;
        int height;
    };
    class Entity;
    struct Material;
    class SpriteModel;

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

            void LoadModelTexture(std::string textureName, std::string texturePath);
            void AddSpriteComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string textureName, bool transparency=false);

        private:
            TextureManager() {}

            std::hash<std::string> mHasher;
            std::unordered_map<std::size_t, std::shared_ptr<SpriteModel> > mTextureRecord;

    };

}