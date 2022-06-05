#include "meshGenerator.hpp"
#include <glad/glad.h>

namespace Journey {

    void MeshGenerator::GeneratePlane(std::shared_ptr<StaticMeshComponent> component, bool light)
    {
        unsigned int tmpVAO, tmpVBO, tmpEBO;

        std::vector<float> vertices;
        if (light) {
            float v[4*6] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        else {
            float v[4*3] = {-0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f,
                             0.5f,  0.5f, 0.0f,
                            -0.5f,  0.5f, 0.0f
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2, 2, 3, 0
        };
        float* vert = &vertices[0];

        glGenVertexArrays(1, &tmpVAO);
        glGenBuffers(1, &tmpVBO);
        glGenBuffers(1, &tmpEBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(tmpVAO);

        glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vert, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (light?6:3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (light) {
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        component->VAO = tmpVAO;
        component->VBO = tmpVBO;
        component->vertices = vert;
        component->vertexCount = 6;
        component->usingIndicesToRender = true;
    }

    void MeshGenerator::GenerateCube(std::shared_ptr<StaticMeshComponent> component, bool light)
    {
        unsigned int tmpVAO, tmpVBO, tmpEBO;
        std::vector<float> vertices;
        if (light) {
            float sq3 = 0.57735027;
            float v[8*6] = {-0.5, -0.5,  0.5, -sq3, -sq3,  sq3,
                             0.5, -0.5,  0.5,  sq3, -sq3,  sq3,
                             0.5,  0.5,  0.5,  sq3,  sq3,  sq3,
                            -0.5,  0.5,  0.5, -sq3,  sq3,  sq3,

                            -0.5, -0.5, -0.5, -sq3, -sq3, -sq3,
                             0.5, -0.5, -0.5,  sq3, -sq3, -sq3,
                             0.5,  0.5, -0.5,  sq3,  sq3, -sq3,
                            -0.5,  0.5, -0.5, -sq3,  sq3, -sq3
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        else {
            float v[8*3] = {-0.5, -0.5,  0.5,
                             0.5, -0.5,  0.5,
                             0.5,  0.5,  0.5,
                            -0.5,  0.5,  0.5,

                            -0.5, -0.5, -0.5,
                             0.5, -0.5, -0.5, 
                             0.5,  0.5, -0.5, 
                            -0.5,  0.5, -0.5
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            4, 5, 1, 1, 0, 4,
            6, 7, 3, 3, 2, 6,
            5, 6, 2, 2, 1, 5,
            7, 4, 0, 0, 3, 7
        };
        float* vert = &vertices[0];

        glGenVertexArrays(1, &tmpVAO);
        glGenBuffers(1, &tmpVBO);
        glGenBuffers(1, &tmpEBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(tmpVAO);

        glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vert, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (light?6:3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (light) {
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        component->VAO = tmpVAO;
        component->VBO = tmpVBO;
        component->vertices = vert;
        component->vertexCount = 36;
        component->usingIndicesToRender = true;
    }

    void MeshGenerator::GenerateCylinder(std::shared_ptr<StaticMeshComponent> component, bool light, int segments)
    {
        unsigned int tmpVAO, tmpVBO, tmpEBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float dTheta = 2*3.14159265358979323846f / segments;

        // Faces of the circles sides
        if (light){
            float v0[2*6] = { 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f,
                          0.0f, 0.0f,  0.5f, 0.0f, 0.0f,  1.0f
            };
            vertices.insert(vertices.end(), std::begin(v0), std::end(v0));
        }
        else {
            float v0[2*3] = { 0.0f, 0.0f, -0.5f,
                          0.0f, 0.0f,  0.5f
            };
            vertices.insert(vertices.end(), std::begin(v0), std::end(v0));
        }
        
        for (int i = 0; i < segments+1; i++) {
            float tempX = 0.5f * glm::cos(i * dTheta);
            float tempY = 0.5f * glm::sin(i * dTheta);
            
            if (light) {
                glm::vec3 sideNormal= {glm::cos(i * dTheta), glm::sin(i * dTheta), 0};
                glm::vec3 lowerNormal = (sideNormal + glm::vec3(0, 0, -1))/2.0f;
                glm::vec3 upperNormal = (sideNormal + glm::vec3(0, 0,  1))/2.0f;
                float v[4*6] = {tempX, tempY, -0.5, lowerNormal.x, lowerNormal.y, lowerNormal.z,
                                tempX, tempY,  0.5, upperNormal.x, upperNormal.y, upperNormal.z,
                                tempX, tempY, -0.5, lowerNormal.x, lowerNormal.y, lowerNormal.z,
                                tempX, tempY,  0.5, upperNormal.x, upperNormal.y, upperNormal.z,
                    };
                vertices.insert(vertices.end(), std::begin(v), std::end(v));
            }
            else {
                float v[4*3] = {tempX, tempY, -0.5,
                                tempX, tempY,  0.5,
                                tempX, tempY, -0.5,
                                tempX, tempY,  0.5,
                    };
                vertices.insert(vertices.end(), std::begin(v), std::end(v));
            }

            if (i != segments) {
                unsigned ind[4*3] = {0    , 4*i+2, 4*i+6,
                                     1    , 4*i+3, 4*i+7,
                                     4*i+4, 4*i+8, 4*i+9,
                                     4*i+9, 4*i+5, 4*i+4
                };
                indices.insert(indices.end(), std::begin(ind), std::end(ind));
            }
        }

        float* vert = &vertices[0];
        unsigned int* indi = &indices[0];

        glGenVertexArrays(1, &tmpVAO);
        glGenBuffers(1, &tmpVBO);
        glGenBuffers(1, &tmpEBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(tmpVAO);

        glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vert, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indi, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (light?6:3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (light) {
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        component->VAO = tmpVAO;
        component->VBO = tmpVBO;
        component->vertices = vert;
        component->vertexCount = indices.size();
        component->usingIndicesToRender = true;
    }

    void MeshGenerator::GenerateSphere(std::shared_ptr<StaticMeshComponent> component, bool light, int segments)
    {

    }

}