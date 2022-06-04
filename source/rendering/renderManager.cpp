#include "renderManager.hpp"
#include <glad/glad.h>
#include "../scene/scene.hpp"
#include "../input/inputController.hpp"


namespace Journey {
    
    RenderManager::RenderManager() {}

    void RenderManager::StartUp() {
        SimpleColoredShader.StartUp("../../../source/rendering/shaders/SimpleColoredShader.vs", "../../../source/rendering/shaders/SimpleColoredShader.fs");
        SimpleTexturedShader.StartUp("../../../source/rendering/shaders/SimpleTexturedShader.vs", "../../../source/rendering/shaders/SimpleTexturedShader.fs");
        FlatColoredShader.StartUp("../../../source/rendering/shaders/FlatColoredShader.vs", "../../../source/rendering/shaders/FlatColoredShader.fs");
        FlatTexturedShader.StartUp("../../../source/rendering/shaders/FlatTexturedShader.vs", "../../../source/rendering/shaders/FlatTexturedShader.fs");
        PhongColoredShader.StartUp("../../../source/rendering/shaders/PhongColoredShader.vs", "../../../source/rendering/shaders/PhongColoredShader.fs");
        PhongTexturedShader.StartUp("../../../source/rendering/shaders/PhongTexturedShader.vs", "../../../source/rendering/shaders/PhongTexturedShader.fs");
        // other shaders
        CleanRenderInfo();

            //Enabling transparencies
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);  
    }

    void RenderManager::ShutDown(){

    }

    void RenderManager::CleanRenderInfo()
    {
        mSimpleColoredObjects.clear();
        mSimpleTexturedObjects.clear();
        mFlatColoredObjects.clear();
        mPhongColoredObjects.clear();
        mFlatTexturedObjects.clear();
        mPhongTexturedObjects.clear();
    }

    void RenderManager::DrawCall(Scene& scene)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mDrawLines)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

          
        // Drawing Guide lines for the Debug mode
        if (scene.InDebugMode())
            DrawDebugObjects(SimpleColoredShader, scene);
        
        // <------ SIMPLE COLORED SHADER ------->
        SimpleColoredShader.use();
        SimpleColoredShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        SimpleColoredShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mSimpleColoredObjects) {
            // bind textures on corresponding texture units
            glBindVertexArray(renderInfo.VAO);
            SimpleColoredShader.setVec3("shapeColor", renderInfo.color);
            SimpleColoredShader.setMat4("model", renderInfo.model);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }  

        // <------ FLAT COLORED SHADER ------->
        FlatColoredShader.use();
        FlatColoredShader.setVec3("light.position", scene.GetPointLight().getLightPos());
        FlatColoredShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        FlatColoredShader.setVec3("light.ambient", scene.GetPointLight().getAmbientColor());
        FlatColoredShader.setVec3("light.diffuse", scene.GetPointLight().getDifuseColor());
        FlatColoredShader.setVec3("light.specular",scene.GetPointLight().getSpecularColor());
        FlatColoredShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        FlatColoredShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mFlatColoredObjects) {
            // material properties
            FlatColoredShader.setVec3("shapeColor", renderInfo.color);
            FlatColoredShader.setVec3("material.ambient", renderInfo.ke);
            FlatColoredShader.setVec3("material.diffuse", renderInfo.kd);
            FlatColoredShader.setVec3("material.specular", renderInfo.ks); 
            FlatColoredShader.setFloat("material.shininess", 64.0f);
            
            FlatColoredShader.setMat4("model", renderInfo.model);
            // render 
            glBindVertexArray(renderInfo.VAO);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }   

        // <------ PHONG COLORED SHADER ------->
        PhongColoredShader.use();
        PhongColoredShader.setVec3("light.position", scene.GetPointLight().getLightPos());
        PhongColoredShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        PhongColoredShader.setVec3("light.ambient", scene.GetPointLight().getAmbientColor());
        PhongColoredShader.setVec3("light.diffuse", scene.GetPointLight().getDifuseColor());
        PhongColoredShader.setVec3("light.specular",scene.GetPointLight().getSpecularColor());
        PhongColoredShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        PhongColoredShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mPhongColoredObjects) {
            // material properties
            PhongColoredShader.setVec3("shapeColor", renderInfo.color);
            PhongColoredShader.setVec3("material.ambient", renderInfo.ke);
            PhongColoredShader.setVec3("material.diffuse", renderInfo.kd);
            PhongColoredShader.setVec3("material.specular", renderInfo.ks); 
            PhongColoredShader.setFloat("material.shininess", 164.0f);
            
            PhongColoredShader.setMat4("model", renderInfo.model);
            // render 
            glBindVertexArray(renderInfo.VAO);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }   

        // <------ FLAT TEXTURED SHADER ------->
        FlatTexturedShader.use();
        FlatTexturedShader.setVec3("light.position", scene.GetPointLight().getLightPos());
        FlatTexturedShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        FlatTexturedShader.setVec3("light.ambient", scene.GetPointLight().getAmbientColor());
        FlatTexturedShader.setVec3("light.diffuse", scene.GetPointLight().getDifuseColor());
        FlatTexturedShader.setVec3("light.specular",scene.GetPointLight().getSpecularColor());
        FlatTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        FlatTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mFlatTexturedObjects) {
            // material properties
            FlatTexturedShader.setVec3("material.ambient", renderInfo.ke);
            FlatTexturedShader.setVec3("material.diffuse", renderInfo.kd);
            FlatTexturedShader.setVec3("material.specular", renderInfo.ks); 
            FlatTexturedShader.setFloat("material.shininess", 64.0f);
            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, renderInfo.textureID);
            glBindVertexArray(renderInfo.VAO);
            FlatTexturedShader.setMat4("model", renderInfo.model);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }   

        // <------ PHONG TEXTURED SHADER ------->
        PhongTexturedShader.use();
        PhongTexturedShader.setVec3("light.position", scene.GetPointLight().getLightPos());
        PhongTexturedShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        PhongTexturedShader.setVec3("light.ambient", scene.GetPointLight().getAmbientColor());
        PhongTexturedShader.setVec3("light.diffuse", scene.GetPointLight().getDifuseColor());
        PhongTexturedShader.setVec3("light.specular",scene.GetPointLight().getSpecularColor());
        PhongTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        PhongTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mPhongTexturedObjects) {
            // material properties
            PhongTexturedShader.setVec3("material.ambient", renderInfo.ke);
            PhongTexturedShader.setVec3("material.diffuse", renderInfo.kd);
            PhongTexturedShader.setVec3("material.specular", renderInfo.ks); 
            PhongTexturedShader.setFloat("material.shininess", 64.0f);
            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, renderInfo.textureID);
            glBindVertexArray(renderInfo.VAO);
            PhongTexturedShader.setMat4("model", renderInfo.model);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }   

        // <------ SIMPLE TEXTURED SHADER ------->
        SimpleTexturedShader.use();
        SimpleTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        SimpleTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mSimpleTexturedObjects) {
            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, renderInfo.textureID);
            glBindVertexArray(renderInfo.VAO);
            SimpleTexturedShader.setMat4("model", renderInfo.model);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.vertexCount);
        }   
    }

    

    void RenderManager::AddObjectToRender(EMaterialType materialType, RenderInfo renderInfo)
    {
        switch(materialType) {
            case EMaterialType::SimpleColored:
                mSimpleColoredObjects.push_back(renderInfo);
                break;
            case EMaterialType::SimpleTextured:
                mSimpleTexturedObjects.push_back(renderInfo);
                break;
            case EMaterialType::FlatColored:
                mFlatColoredObjects.push_back(renderInfo);
                break;
            case EMaterialType::PhongColored:
                mPhongColoredObjects.push_back(renderInfo);
                break;
            case EMaterialType::FlatTextured:
                mFlatTexturedObjects.push_back(renderInfo);
                break;
            case EMaterialType::PhongTextured:
                mPhongTexturedObjects.push_back(renderInfo);
                break;
            default:
                // code block
                break;
        }               
    }

    void RenderManager::CreateDebugObjects()
    {
        CreateDebugFrustrumObject();
        CreateDebugAxisObject();
    }

    void RenderManager::CreateDebugFrustrumObject()
    {
        // Debug Lines
        RenderInfo rInfo;
        unsigned int tmpVBO;
        unsigned int tmpEBO;
        float vertices[] = {
            // positions           
            -1.0f,-1.0f,  -1.0f,      
            1.0f, -1.0f,  -1.0f,      
            1.0f , 1.0f, -1.0f,     
            -1.0f, 1.0f, -1.0f,     
             
            -1.0f, -1.0f, 1.0f, 
            1.0f, -1.0f,  1.0f, 
            1.0f, 1.0f, 1.0f, 
            -1.0f,1.0f, 1.0f
        };

        unsigned int indices[] = {  // note that we start from 0!
            0, 1,   4, 5,
            1, 2,   5, 6,
            2, 3,   6, 7,
            3, 0,   7, 4,

            0, 4,
            1, 5,
            2, 6,
            3, 7
        };

        glGenVertexArrays(1, &rInfo.VAO);
        glGenBuffers(1, &tmpVBO);
        glGenBuffers(1, &tmpEBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(rInfo.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        rInfo.vertexCount = 24;
        rInfo.usingIndices = true;
        mDebugObjects.push_back(rInfo);
    }
    
    void RenderManager::CreateDebugAxisObject()
    {

    }
    
    void RenderManager::DrawDebugObjects(Shader shaderProgram, Scene& scene)
    {
        if (mDebugObjects.size() > 0)
        {
           mDebugObjects.at(0).model = glm::inverse(scene.GetInputController().GetLastCamera()->getProjection() * scene.GetInputController().GetLastCamera()->getViewMatrix());
        }

        shaderProgram.use();
        shaderProgram.setMat4("projection", scene.GetCameraHandler().getProjection());
        shaderProgram.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mDebugObjects) {   
            shaderProgram.setVec3("shapeColor", glm::vec3(1.0f, 1.0f, 0.0f));
            shaderProgram.setMat4("model", renderInfo.model);
            if (renderInfo.usingIndices){
                glBindVertexArray(renderInfo.VAO);
                glDrawElements(GL_LINES, renderInfo.vertexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            else {
                glBindVertexArray(renderInfo.VAO);
                glDrawArrays(GL_LINES, 0, renderInfo.vertexCount);
            }
        }
    }
}