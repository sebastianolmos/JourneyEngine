#include "animator.hpp"


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "animation.hpp"
#include "bone.hpp"
#include "animdata.hpp"
#include <iostream>

namespace Journey 
{
    Animator::Animator()
    {
		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    Animator::Animator(std::shared_ptr<Animation> animation)
    {
        m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    bool Animator::UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
		if ((m_CurrentAnimation || m_Playing) && ! m_Finished)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            if (!m_CurrentAnimation->isLooping() && m_CurrentTime > m_CurrentAnimation->GetDuration() )
            {
                m_Finished = true;
                m_Playing = false;
                m_CurrentTime = 0.0f;
                return true;
            }
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
        return false;
    }

    void Animator::PlayAnimation(std::shared_ptr<Animation> pAnimation)
    {
        m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
    }

    void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
    }

    std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

} 