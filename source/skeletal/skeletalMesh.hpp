#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

namespace Journey {

    #define MAX_BONE_INFLUENCE 4

    class Shader;

    struct skVertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct skTexture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class SkeletalMesh {
    public:
        // mesh Data
        std::vector<skVertex>       vertices;
        std::vector<unsigned int>   indices;
        std::vector<skTexture>      textures;
        unsigned int VAO;

        SkeletalMesh(std::vector<skVertex> vertices, std::vector<unsigned int> indices, std::vector<skTexture> textures);
        void Draw(Shader &shader);

    private:
        // render data 
        unsigned int VBO, EBO;
        // initializes all the buffer objects/arrays
        void setupMesh();
    };


}