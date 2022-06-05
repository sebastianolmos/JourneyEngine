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

    }

    void MeshGenerator::GenerateSphere(std::shared_ptr<StaticMeshComponent> component, bool light, int segments)
    {

    }

}