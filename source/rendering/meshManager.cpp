#include "meshManager.hpp"

#include "../entity/entity.hpp"
#include "../component/staticMesh.hpp"
#include "material.hpp"

namespace Journey
{

    void MeshManager::LoadPrimitiveMeshToEntity(std::shared_ptr<Entity> entity, std::shared_ptr<Material> mat, EPrimitiveMesh primitiveMesh)
    {
        if (entity == nullptr || entity->mComponents.count(EComponentType::SpriteComponent) > 0)
            return;
        
        switch(primitiveMesh) {
            case EPrimitiveMesh::Cube: {
                if (mat->GetType() == EMaterialType::SimpleColored) {
                    std::cout << "Material NOT Implemented yet" << std::endl;
                } 
                else if (mat->GetType() == EMaterialType::PhongColored || mat->GetType() == EMaterialType::FlatColored) {
                    std::shared_ptr<StaticMeshComponent> meshComp = std::make_shared<StaticMeshComponent>();
                    float vertices[] = {
                        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
                    };
                    // first, configure the cube's VAO (and VBO)
                    unsigned int VBO, cubeVAO;
                    glGenVertexArrays(1, &cubeVAO);
                    glGenBuffers(1, &VBO);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glBindVertexArray(cubeVAO);

                    // position attribute
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);
                    // normal attribute
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
                    glEnableVertexAttribArray(1);

                    meshComp->VAO = cubeVAO;
                    meshComp->VBO = VBO;
                    meshComp->vertices = vertices;
                    meshComp->vertexCount = 36;
                    meshComp->vertexStride = 6;

                    meshComp->material = mat; 
                    entity->mComponents.insert(std::make_pair(EComponentType::StaticMeshComponent, meshComp));
                } 
                else {
                    std::cout << "Material not alowed with PrimitiveMEsh" << std::endl;
                }
                break;
                }
            case EPrimitiveMesh::Plane: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Pyramid: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Cylinder: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Sphere: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            case EPrimitiveMesh::Capsule: {
                std::cout << "Primitive mesh NOT Implemented yet" << std::endl;
                break;
                }
            }
    }

}