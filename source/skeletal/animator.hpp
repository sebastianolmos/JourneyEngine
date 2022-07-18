#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace Journey {
    class Animation;
    struct AssimpNodeData;

    class Animator {
    public:
        Animator(Animation* animation);
        void UpdateAnimation(float dt);
        void PlayAnimation(Animation* pAnimation);
        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
        std::vector<glm::mat4> GetFinalBoneMatrices();

    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;
    };

}