#include "renderManager.hpp"
#include <glad/glad.h>
#include "../scene/scene.hpp"


namespace Journey {
    
    RenderManager::RenderManager() {}

    void RenderManager::StartUp() {
        PhongColoredShader.StartUp("../../../source/rendering/shaders/phongColoredShader.vs", "../../../source/rendering/shaders/phongColoredShader.fs");
        SimpleTexturedShader.StartUp("../../../source/rendering/shaders/SimpleTexturedShader.vs", "../../../source/rendering/shaders/SimpleTexturedShader.fs");
        // other shaders
        CleanRenderInfo();

            //Enabling transparencies
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
            PhongColoredShader.setVec3("material.ambient", renderInfo.ke);
            PhongColoredShader.setVec3("material.diffuse", renderInfo.kd);
            PhongColoredShader.setVec3("material.specular", renderInfo.ks); 
            PhongColoredShader.setFloat("material.shininess", 64.0f);
            
            PhongColoredShader.setMat4("model", renderInfo.model);
            // render 
            glBindVertexArray(renderInfo.VAO);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.faces);
        }   


        SimpleTexturedShader.use();
        SimpleTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        SimpleTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());

        for(auto& renderInfo: mSimpleTexturedObjects) {
            
            // bind textures on corresponding texture units
            glBindTexture(GL_TEXTURE_2D, renderInfo.textureID);
            glBindVertexArray(renderInfo.VAO);
            SimpleTexturedShader.setMat4("model", renderInfo.model);
            glDrawArrays(GL_TRIANGLES, 0, renderInfo.faces);
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
}