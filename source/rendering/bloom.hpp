#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Journey {
    class Shader;
    class RenderManager;

    struct bloomMip {
        glm::vec2 size;
        glm::ivec2 intSize;
        unsigned int texture;
    };

    class bloomFBO{
    public:
        bloomFBO();
        ~bloomFBO();
        bool Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength);
        void Destroy();
        void BindForWriting();
        const std::vector<bloomMip>& MipChain() const;

    private:
        bool mInit;
        unsigned int mFBO;
        std::vector<bloomMip> mMipChain;
    };

    class BloomRenderer {
    public:
        BloomRenderer();
        ~BloomRenderer();
        bool Init(unsigned int windowWidth, unsigned int windowHeight);
        void Destroy();
        void RenderBloomTexture(unsigned int srcTexture, float filterRadius);
        unsigned int BloomTexture();
        unsigned int BloomMip_i(int index);

    private:
        friend RenderManager;
        void RenderDownsamples(unsigned int srcTexture);
        void RenderUpsamples(float filterRadius);
        void renderQuad();

        bool mInit;
        bloomFBO mFBO;
        glm::ivec2 mSrcViewportSize;
        glm::vec2 mSrcViewportSizeFloat;
        Shader* mDownsampleShader;
        Shader* mUpsampleShader;

        bool mKarisAverageOnDownsample = true;
        unsigned int quadVAO = 0;
        unsigned int quadVBO;
    };

}