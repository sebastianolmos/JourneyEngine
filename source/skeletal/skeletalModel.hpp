#pragma once

#include <map>
#include <string>
#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace Journey {
    class skTexture;
    class SkeletalMesh;
    class Shader;
    struct BoneInfo;
    struct skVertex;

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

    private:
        std::map<std::string, BoneInfo> m_BoneInfoMap;
	    int m_BoneCounter = 0;

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