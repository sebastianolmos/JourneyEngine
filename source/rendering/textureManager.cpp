#include "textureManager.hpp"
#include <glad/glad.h>

#include <stb_image.h>
#include <iostream>

#include "../entity/entity.hpp"
#include "../component/sprite.hpp"
#include "material.hpp"
#include "model.hpp"

namespace Journey {

    void TextureManager::LoadModelTexture(std::string textureName, std::string texturePath) 
    {
        std::size_t hashedName = mHasher(textureName);
        if (mTextureRecord.count(hashedName) != 0)
            return;

        float texvertices[] = {
            -0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
            0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
            0.5f, 0.0f, 0.5f,  1.0f, 0.0f,
            0.5f,  0.0f, 0.5f,  1.0f, 0.0f,
            -0.5f,  0.0f, 0.5f,  0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,  0.0f, 1.0f
        };
        unsigned int texVBO, texVAO;
        glGenVertexArrays(1, &texVAO);
        glGenBuffers(1, &texVBO);

        glBindVertexArray(texVAO);

        glBindBuffer(GL_ARRAY_BUFFER, texVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texvertices), texvertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        std::shared_ptr<TextureRep> texPtr = LoadTexture(texturePath, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
        std::shared_ptr<SpriteModel> model = std::make_shared<SpriteModel>();
        model->mVAO = texVAO;
        model->mVertices = texvertices;
        model->mVertexCount = 6;
        model->mTextureID = texPtr->textureId;
        model->mWidth = texPtr->width;
        model->mHeight = texPtr->height;
        mTextureRecord.insert({hashedName, model});
    }   
    
    void TextureManager::AddSpriteComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string textureName, bool transparency) 
    {
        std::size_t hashedName = mHasher(textureName);
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) != 0)
            return;
        if (mTextureRecord.count(hashedName) == 0) {
            std::cout << textureName << " No registered"<< std::endl;
            return;
        }

        std::shared_ptr<SpriteComponent> spriteComp = std::make_shared<SpriteComponent>();
        spriteComp->spriteModel = mTextureRecord[hashedName];
        spriteComp->material = mat; 
        spriteComp->transparency = transparency;
        entity->mComponents.insert(std::make_pair(EComponentType::SpriteComponent, spriteComp));
    }


    std::shared_ptr<TextureRep> TextureManager::LoadTexture(const std::string filePath, int magFilter, int minFilter,
                                                        int sWrapMode,  int tWrapMode)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        TextureRep* returnTexture = new TextureRep{texture, width, height};
        std::shared_ptr<TextureRep> texturePtr = std::shared_ptr<TextureRep>(returnTexture);
        return texturePtr;
    }

}
