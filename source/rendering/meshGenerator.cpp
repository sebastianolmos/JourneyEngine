#include "meshGenerator.hpp"
#include <glad/glad.h>

#include "model.hpp"

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
        std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
        model->mVAO = tmpVAO;
        model->mVertices = vert;
        model->mIndexCount = 6;
        component->meshModel = model;
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
        std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
        model->mVAO = tmpVAO;
        model->mVertices = vert;
        model->mIndexCount = 36;
        component->meshModel = model;
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
        std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
        model->mVAO = tmpVAO;
        model->mVertices = vert;
        model->mIndexCount = indices.size();
        component->meshModel = model;
    }

    void MeshGenerator::GenerateSphere(std::shared_ptr<StaticMeshComponent> component, bool light, int segments)
    {
        unsigned int tmpVAO, tmpVBO, tmpEBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float dTheta = 2*3.14159265358979323846f / segments;
        int c = 0;

        for (int i = 0; i < segments/2; i++) {
            float theta = i *dTheta;
            float theta1 = (i+1)*dTheta;
            for (int j = 0; j < segments; j++) {
                float phi = j *dTheta;
                float phi1 = (j+1)*dTheta;
                glm::vec3 v0 = glm::vec3(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta))*0.5f;
                glm::vec3 v1 = glm::vec3(glm::sin(theta1)*glm::cos(phi), glm::sin(theta1)*glm::sin(phi), glm::cos(theta1))*0.5f;
                glm::vec3 v2 = glm::vec3(glm::sin(theta1)*glm::cos(phi1), glm::sin(theta1)*glm::sin(phi1), glm::cos(theta1))*0.5f;
                glm::vec3 v3 = glm::vec3(glm::sin(theta)*glm::cos(phi1), glm::sin(theta)*glm::sin(phi1), glm::cos(theta))*0.5f;

                if (light) {
                    glm::vec3 n0 = glm::vec3(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta));
                    glm::vec3 n1 = glm::vec3(glm::sin(theta1)*glm::cos(phi), glm::sin(theta1)*glm::sin(phi), glm::cos(theta1));
                    glm::vec3 n2 = glm::vec3(glm::sin(theta1)*glm::cos(phi1), glm::sin(theta1)*glm::sin(phi1), glm::cos(theta1));
                    glm::vec3 n3 = glm::vec3(glm::sin(theta)*glm::cos(phi1), glm::sin(theta)*glm::sin(phi1), glm::cos(theta));
                    float v[4*6] = {v0.x, v0.y, v0.z, n0.x, n0.y, n0.z,
                                    v1.x, v1.y, v1.z, n1.x, n1.y, n1.z,
                                    v2.x, v2.y, v2.z, n2.x, n2.y, n2.z,
                                    v3.x, v3.y, v3.z, n3.x, n3.y, n3.z
                        };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                } 
                else {
                    float v[4*3] = {v0.x, v0.y, v0.z,
                                    v1.x, v1.y, v1.z,
                                    v2.x, v2.y, v2.z,
                                    v3.x, v3.y, v3.z,
                        };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                unsigned ind[2*3] = { c + 0, c + 1, c + 2,
                                      c + 2, c + 3, c + 0
                };
                indices.insert(indices.end(), std::begin(ind), std::end(ind));
                c += 4;
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
        std::shared_ptr<StaticMeshModel> model = std::make_shared<StaticMeshModel>();
        model->mVAO = tmpVAO;
        model->mVertices = vert;
        model->mIndexCount = indices.size();
        component->meshModel = model;
    }

}