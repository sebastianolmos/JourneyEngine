#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <memory>

namespace Journey {
    class Animation;
    struct AssimpNodeData;
    class SkeletalModel;

    class Animator {
    public:
        Animator();
        Animator(std::shared_ptr<Animation> animation);
        bool UpdateAnimation(float dt);
        void PlayAnimation(std::shared_ptr<Animation> pAnimation);
        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
        std::vector<glm::mat4> GetFinalBoneMatrices();

    private:
        friend SkeletalModel;
        std::vector<glm::mat4> m_FinalBoneMatrices;
        std::shared_ptr<Animation> m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;
        bool m_Finished;
        bool m_Playing;
    };

}