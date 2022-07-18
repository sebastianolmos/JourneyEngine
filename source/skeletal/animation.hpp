#pragma once 

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <string>

struct aiAnimation;
struct aiNode;

namespace Journey {
    class SkeletalModel;
    class Bone;
    struct BoneInfo;

    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::string name;
        int childrenCount;
        std::vector<AssimpNodeData> children;
    };

    class Animation {
    public:
        Animation() = default;
        Animation(const std::string& animationPath, SkeletalModel* model);
        Animation(const std::string& animationPath, SkeletalModel* model, unsigned int index);
        ~Animation()
        {
        }

        Bone* FindBone(const std::string& name);
        inline float GetTicksPerSecond() { return m_TicksPerSecond; }
        inline float GetDuration() { return m_Duration;}
        inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
        inline const std::map<std::string,BoneInfo>& GetBoneIDMap() 
        { 
            return m_BoneInfoMap;
        }
        bool isLooping() {
            return mLooping;
        }
        void setLooping(bool value) {
            mLooping = value;
        }

    private:
        void ReadMissingBones(const aiAnimation* animation, SkeletalModel& model);
        void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

        float m_Duration;
        int m_TicksPerSecond;
        std::vector<Bone> m_Bones;
        AssimpNodeData m_RootNode;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        bool mLooping = false;

    };

}