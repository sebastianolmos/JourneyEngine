#include "textureManager.hpp"
#include <stb_image.h>
#include <iostream>

#include "../entity/entity.hpp"
#include "../component/sprite.hpp"
#include "material.hpp"

namespace Journey {
    std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string filePath, GLint magFilter, GLint minFilter,
                                                        GLint sWrapMode,  GLint tWrapMode)
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

        Texture* returnTexture = new Texture{texture, width, height};
        std::shared_ptr<Texture> texturePtr = std::shared_ptr<Texture>(returnTexture);
        return texturePtr;
    }

    void TextureManager::LoadSpriteToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, std::string spritePath)
    {
        if (entity == nullptr)
            return;
        
        if (entity->mComponents.count(EComponentType::SpriteComponent) == 0) {
            
            switch (mat->GetType())
            {
            case EMaterialType::SimpleTextured: {
                std::shared_ptr<SpriteComponent> spriteComp = std::make_shared<SpriteComponent>();
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

                spriteComp->VAO = texVAO;
                spriteComp->VBO = texVBO;
                spriteComp->vertices = texvertices;
                spriteComp->vertexCount = 6;
                spriteComp->vertexStride = 5;

                std::shared_ptr<Texture> texPtr = LoadTexture(spritePath, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
                spriteComp->textureID = texPtr->textureId;
                spriteComp->material = mat; 
                entity->mComponents.insert(std::make_pair(EComponentType::SpriteComponent, spriteComp));
                break;
                }
            case EMaterialType::FlatTextured: {
                std::cout << "NOT Implemented yet" << std::endl;
                break;
                }
            case EMaterialType::PhongTextured: {
                std::cout << "NOT Implemented yet" << std::endl;
                break;
                }
            default: {
                std::cout << "Material not alowed to an Sprite" << std::endl;
                break;
                }
            }
        }
    }

}
