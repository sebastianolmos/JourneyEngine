#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


#include <assimp/scene.h>
#include "skeletalMesh.hpp"
#include "animdata.hpp"

struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace Journey {
    class skTexture;
    class Shader;
    struct skVertex;
    class Animator;
    class SkeletalManager;
    class Animation;



    class SkeletalModel {
    public:
        std::vector<skTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<SkeletalMesh>    meshes;
        std::string directory;
        bool gammaCorrection = true;

        SkeletalModel(std::string const &path, bool gamma = true);

        void Draw(Shader &shader);
        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	    int& GetBoneCount() { return m_BoneCounter; }

        void LoadAnimation(unsigned int index, std::string name, bool loop = false);
        void PlayAnimation(std::string name);
        void PauseCurrentAnimation();
        void StopCurrentAnimation();
        bool IsPlayingCurrentAnimation();
        bool IsPausedCurrentAnimation();
        bool IsFinishedCurrentAnimation();
        bool IsLoopingCurrentAnimation();
        void SetLoopingCurrentAnimation(bool value);

    private:
        friend SkeletalManager;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
	    int m_BoneCounter = 0;
        // Animation Handler
        Animator* mAnimator;
        std::hash<std::string> mHasher;
        std::unordered_map<std::size_t, std::shared_ptr<Animation> > mAnimationRecord;
        aiNode* mRootNode;
        aiAnimation** mRawAnimations;
        std::string mPath;
        std::shared_ptr<Animation> lastAnimation;

        void UpdateAnimator(float deltaTime);
        void loadModel(std::string const &path);
        void processNode(aiNode *node, const aiScene *scene);
        void SetVertexBoneDataToDefault(skVertex& vertex);
        SkeletalMesh processMesh(aiMesh* mesh, const aiScene* scene);
        void SetVertexBoneData(skVertex& vertex, int boneID, float weight);
        void ExtractBoneWeightForVertices(std::vector<skVertex>& vertices, aiMesh* mesh, const aiScene* scene);
        unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = true);

        std::vector<skTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };

}