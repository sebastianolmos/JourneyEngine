#pragma once

#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

class Shader;
namespace Journey {

    struct Vertex {
        glm::vec3 Position; // position
        glm::vec3 Normal; // normal
        glm::vec2 TexCoords; // texCoords
        glm::vec3 Tangent; // tangent
        glm::vec3 Bitangent; // bitangent
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {

    public:
        // constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);

        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO;

    private:
        // render data 
        unsigned int VBO, EBO;
        void setupMesh();

    };
    
}

#endif