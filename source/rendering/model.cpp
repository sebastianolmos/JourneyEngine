#include "model.hpp"

#include <glad/glad.h>

namespace Journey {

    void StaticMeshModel::drawCall(Shader& shader)
    {
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void SpriteModel::drawCall(Shader& shader)
    {
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
    }

}