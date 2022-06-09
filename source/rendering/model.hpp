#pragma once

namespace Journey {
    class Shader;
    class MeshGenerator;
    class TextureManager;

    class Model {
    public:
        virtual void drawCall(Shader& shader) = 0;
    };

    class StaticMeshModel : public Model {
    public:
        StaticMeshModel() = default;
        virtual void drawCall(Shader& shader) override;
    private:
        friend MeshGenerator;
        float* mVertices;
        int mIndexCount;
        unsigned int mVAO;
    };

    class SpriteModel : public Model {
    public:
        SpriteModel() = default;
        virtual void drawCall(Shader& shader) override;
    private:
        friend TextureManager;

        float* mVertices;
        int mVertexCount;
        unsigned int mVAO;
        unsigned mTextureID;
        int mWidth;
        int mHeight;
    };
}