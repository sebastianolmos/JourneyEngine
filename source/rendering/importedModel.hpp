#pragma once

#ifndef IMPORTED_MODEL_H
#define IMPORTED_MODEL_H

#include "mesh.hpp"
#include "model.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

struct aiMesh;
struct aiScene;
struct aiNode;
struct aiMaterial;
enum aiTextureType;

namespace Journey {

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    class Shader;

    class ImportedModel: public Model {
    public:
        // constructor, expects a filepath to a 3D model.
        ImportedModel(std::string const& path, bool gamma = false);
        // draws the model, and thus all its meshes
        virtual void drawCall(Shader& shader) override;

        // model data 
        std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>    meshes;
        std::string directory;
        bool gammaCorrection;

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const& path);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);
        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        
    };

}

#endif



