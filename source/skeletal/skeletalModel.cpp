#include "skeletalModel.hpp"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../rendering/shaders/shader.hpp"
#include "animator.hpp"
#include "animation.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "assimp_glm_helpers.hpp"

namespace Journey {

    SkeletalModel::SkeletalModel(std::string const &path, bool gamma) : gammaCorrection(gamma)
    {
        loadModel(path);
        mAnimator = new Animator();
    }

    void SkeletalModel::Draw(Shader &shader)
    {
        auto transforms = mAnimator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void SkeletalModel::loadModel(std::string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        mRootNode = scene->mRootNode;
        mRawAnimations = scene->mAnimations;
        mPath = path;

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    void SkeletalModel::processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void SkeletalModel::SetVertexBoneDataToDefault(skVertex& vertex)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
    }

    SkeletalMesh SkeletalModel::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<skVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<skTexture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			skVertex vertex;
			SetVertexBoneDataToDefault(vertex);
			vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
			vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
			
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<skTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<skTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<skTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		std::vector<skTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		ExtractBoneWeightForVertices(vertices,mesh,scene);

		return SkeletalMesh(vertices, indices, textures);
    }

    void SkeletalModel::SetVertexBoneData(skVertex& vertex, int boneID, float weight)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_Weights[i] = weight;
				vertex.m_BoneIDs[i] = boneID;
				break;
			}
		}
    }

    void SkeletalModel::ExtractBoneWeightForVertices(std::vector<skVertex>& vertices, aiMesh* mesh, const aiScene* scene)
    {
        auto& boneInfoMap = m_BoneInfoMap;
		int& boneCount = m_BoneCounter;

		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = boneCount;
				newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				boneInfoMap[boneName] = newBoneInfo;
				boneID = boneCount;
				boneCount++;
			}
			else
			{
				boneID = boneInfoMap[boneName].id;
			}
			assert(boneID != -1);
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= vertices.size());
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
    }

    unsigned int SkeletalModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
    {
        std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
            GLenum internalFormat;
            GLenum dataFormat;
            if (nrComponents == 1)
            {
                internalFormat = dataFormat = GL_RED;
            }
            else if (nrComponents == 3)
            {
                internalFormat = gamma ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            }

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<skTexture> SkeletalModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<skTexture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                skTexture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    void SkeletalModel::LoadAnimation(unsigned int index, std::string name, bool loop)
    {
        std::size_t hashedName = mHasher(name);
        if (mAnimationRecord.count(hashedName) != 0)
            return;
        std::shared_ptr<Animation> anim = std::make_shared<Animation>(mPath, this, index);
        anim->setLooping(loop);
        mAnimationRecord.insert({hashedName, anim});
    }

    void SkeletalModel::PlayAnimation(std::string name)
    {
        std::size_t hashedName = mHasher(name);
        if (mAnimationRecord.count(hashedName) == 0){
            std::cout << name << " animación no registrada"<< std::endl;
            return;
        }
        lastAnimation = mAnimator->m_CurrentAnimation;
        mAnimator->m_Playing = true;
        mAnimator->m_Finished = false;
        mAnimator->PlayAnimation(mAnimationRecord[hashedName]);
    }
    
    void SkeletalModel::PauseCurrentAnimation()
    {
        mAnimator->m_Playing = false;
        mAnimator->m_Finished = false;
    }
    
    void SkeletalModel::StopCurrentAnimation()
    {
        mAnimator->m_Playing = false;
        mAnimator->m_Finished = true;
        mAnimator->m_CurrentTime = 0.0f;
    }
    
    bool SkeletalModel::IsPlayingCurrentAnimation()
    {
        return mAnimator->m_Playing;
    }
    
    
    bool SkeletalModel::IsPausedCurrentAnimation()
    {
        return !mAnimator->m_Playing && !mAnimator->m_Finished;
    }

    bool SkeletalModel::IsFinishedCurrentAnimation()
    {
        return mAnimator->m_Finished;
    }

    bool SkeletalModel::IsLoopingCurrentAnimation()
    {
        if (mAnimator->m_CurrentAnimation)
            return mAnimator->m_CurrentAnimation->isLooping();
    }
    
    void SkeletalModel::SetLoopingCurrentAnimation(bool value)
    {
        if (mAnimator->m_CurrentAnimation)
            mAnimator->m_CurrentAnimation->setLooping(value);
    }
    
    void SkeletalModel::UpdateAnimator(float deltaTime)
    {
        bool finished = mAnimator->UpdateAnimation(deltaTime);
        if (finished) {
            auto tempAnim = mAnimator->m_CurrentAnimation;
            if (lastAnimation != nullptr) {
                mAnimator->m_Playing = true;
                mAnimator->m_Finished = false;
                mAnimator->PlayAnimation(lastAnimation);
                lastAnimation = nullptr;
            }
        }
    }
    
}