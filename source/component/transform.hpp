#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Journey {

    class Transform {
        public:
            Transform(const glm::vec3& translation = glm::vec3(0.0f),
                    const glm::fquat& rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f),
                    const glm::vec3& scale = glm::vec3(1.0f))
            {
                mLocalTranslation = translation;
                mLocalScale = scale;
                mLocalRotation = rotation;
            }

            const glm::vec3& GetLocalTranslation();
		    const glm::fquat& GetLocalRotation() const;
		    const glm::vec3& GetLocalScale() const;
		    void SetTranslation(const glm::vec3 translation);
		    void SetRotation(const glm::fquat& rotation);
            void SetScale(const glm::vec3& scale);
            void Set(const glm::vec3 translation, const glm::fquat& rotation, const glm::vec3& scale);

            glm::mat4 GetModelMatrix() const;

        private:
            glm::vec3 mLocalTranslation;
            glm::fquat mLocalRotation;
            glm::vec3 mLocalScale;
            
    };

}