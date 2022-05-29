#include "transform.hpp"

namespace Journey {

    const glm::vec3& Transform::GetLocalTranslation()
    {
        return mLocalTranslation;
    }

    const glm::vec3& Transform::GetLocalScale() const
    {
        return mLocalScale;
    }

    const glm::fquat& Transform::GetLocalRotation() const
    {
        return mLocalRotation;
    }

    void Transform::SetTranslation(const glm::vec3 translation)
    {
        mLocalTranslation = translation;
    }

    void Transform::SetScale(const glm::vec3& scale)
    {
        mLocalScale = scale;
    }

    void Transform::SetRotation(const glm::fquat& rotation)
    {
        mLocalRotation = rotation;
    }

    void Transform::SetRotation(const glm::vec3& rotation)
    {
        mLocalRotation = glm::quat(rotation);
    }

    glm::mat4 Transform::GetModelMatrix() const {
        const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mLocalTranslation);
        const glm::mat4 rotationMatrix = glm::toMat4(mLocalRotation);
        const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), mLocalScale);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    void Transform::Set(const glm::vec3 translation, const glm::fquat& rotation, const glm::vec3& scale)
    {
        mLocalTranslation = translation;
        mLocalScale = scale;
        mLocalRotation = rotation;
    }

    void Transform::Set(const glm::vec3 translation, const glm::vec3& rotation, const glm::vec3& scale)
    {
        mLocalTranslation = translation;
        mLocalScale = scale;
        mLocalRotation = glm::quat(rotation);
    }

}