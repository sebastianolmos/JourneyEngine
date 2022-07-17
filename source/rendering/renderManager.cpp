#include "renderManager.hpp"
#include <glad/glad.h>
#include "../scene/scene.hpp"
#include "../input/inputController.hpp"
#include "../component/pointLight.hpp"
#include "../component/spotLight.hpp"
#include "skybox.hpp"


namespace Journey {
    
    RenderManager::RenderManager() {}

    void RenderManager::StartUp() {
        SimpleColoredShader.StartUp("../../../source/rendering/shaders/SimpleColoredShader.vs", 
                        "../../../source/rendering/shaders/SimpleColoredShader.fs");
        SimpleTexturedShader.StartUp("../../../source/rendering/shaders/SimpleTexturedShader.vs", 
                        "../../../source/rendering/shaders/SimpleTexturedShader.fs");
        FlatColoredShader.StartUp("../../../source/rendering/shaders/FlatColoredShader.vs", 
                        "../../../source/rendering/shaders/FlatColoredShader.fs", MaxPointLights, MaxSpotLights);
        FlatTexturedShader.StartUp("../../../source/rendering/shaders/FlatTexturedShader.vs", 
                        "../../../source/rendering/shaders/FlatTexturedShader.fs", MaxPointLights, MaxSpotLights);
        PhongColoredShader.StartUp("../../../source/rendering/shaders/PhongColoredShader.vs", 
                        "../../../source/rendering/shaders/PhongColoredShader.fs", MaxPointLights, MaxSpotLights);
        PhongTexturedShader.StartUp("../../../source/rendering/shaders/PhongTexturedShader.vs", 
                        "../../../source/rendering/shaders/PhongTexturedShader.fs", MaxPointLights, MaxSpotLights);
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
        if (!mTransparentObjects.empty())
            std::cout << " Something wrong, tranaperency queue isnt empty" << std::endl;
        mPointLights.clear();
        mSpotLights.clear();
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
            SimpleColoredShader.setVec3("shapeColor", renderInfo.color);
            SimpleColoredShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(SimpleColoredShader);            
        }  

        // <------ FLAT COLORED SHADER ------->
        FlatColoredShader.use();
        for (int i = 0; i < mPointLights.size(); i++) {
            std::string number = std::to_string(i);
            FlatColoredShader.setVec3("pointLights["+ number + "].position", mPointLights[i].first);
            FlatColoredShader.setVec3("pointLights["+ number + "].ambient", mPointLights[i].second->mAmbient);
            FlatColoredShader.setVec3("pointLights["+ number + "].diffuse", mPointLights[i].second->mDiffuse);
            FlatColoredShader.setVec3("pointLights["+ number + "].specular", mPointLights[i].second->mSpecular);
            FlatColoredShader.setFloat("pointLights["+ number + "].constant", mPointLights[i].second->mConstant);
            FlatColoredShader.setFloat("pointLights["+ number + "].linear", mPointLights[i].second->mLinear);
            FlatColoredShader.setFloat("pointLights["+ number + "].quadratic", mPointLights[i].second->mQuadratic);
            FlatColoredShader.setBool("pointLights["+ number + "].on", mPointLights[i].second->mOn);
        }
        for (int i = 0; i < mSpotLights.size(); i++) {
            std::string number = std::to_string(i);
            FlatColoredShader.setVec3("spotLights["+ number + "].position", mSpotLights[i].position);
            FlatColoredShader.setVec3("spotLights["+ number + "].direction", mSpotLights[i].direction);
            FlatColoredShader.setVec3("spotLights["+ number + "].ambient", mSpotLights[i].light->mAmbient);
            FlatColoredShader.setVec3("spotLights["+ number + "].diffuse", mSpotLights[i].light->mDiffuse);
            FlatColoredShader.setVec3("spotLights["+ number + "].specular", mSpotLights[i].light->mSpecular);
            FlatColoredShader.setFloat("spotLights["+ number + "].cutOff", mSpotLights[i].light->mCutOff);
            FlatColoredShader.setFloat("spotLights["+ number + "].outerCutOff", mSpotLights[i].light->mOuterCutOff);
            FlatColoredShader.setFloat("spotLights["+ number + "].constant", mSpotLights[i].light->mConstant);
            FlatColoredShader.setFloat("spotLights["+ number + "].linear", mSpotLights[i].light->mLinear);
            FlatColoredShader.setFloat("spotLights["+ number + "].quadratic", mSpotLights[i].light->mQuadratic);
            FlatColoredShader.setBool("spotLights["+ number + "].on",mSpotLights[i].light->mOn);	
        }
        FlatColoredShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        FlatColoredShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        FlatColoredShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mFlatColoredObjects) {
            // material properties
            FlatColoredShader.setVec3("shapeColor", renderInfo.color);
            FlatColoredShader.setVec3("material.ambient", renderInfo.ke);
            FlatColoredShader.setVec3("material.diffuse", renderInfo.kd);
            FlatColoredShader.setVec3("material.specular", renderInfo.ks); 
            FlatColoredShader.setFloat("material.shininess", 64.0f);
            
            FlatColoredShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(FlatColoredShader);
        }   

        // <------ PHONG COLORED SHADER ------->
        PhongColoredShader.use();
        for (int i = 0; i < mPointLights.size(); i++) {
            std::string number = std::to_string(i);
            PhongColoredShader.setVec3("pointLights["+ number + "].position", mPointLights[i].first);
            PhongColoredShader.setVec3("pointLights["+ number + "].ambient", mPointLights[i].second->mAmbient);
            PhongColoredShader.setVec3("pointLights["+ number + "].diffuse", mPointLights[i].second->mDiffuse);
            PhongColoredShader.setVec3("pointLights["+ number + "].specular", mPointLights[i].second->mSpecular);
            PhongColoredShader.setFloat("pointLights["+ number + "].constant", mPointLights[i].second->mConstant);
            PhongColoredShader.setFloat("pointLights["+ number + "].linear", mPointLights[i].second->mLinear);
            PhongColoredShader.setFloat("pointLights["+ number + "].quadratic", mPointLights[i].second->mQuadratic);
            PhongColoredShader.setBool("pointLights["+ number + "].on", mPointLights[i].second->mOn);
        }
        for (int i = 0; i < mSpotLights.size(); i++) {
            std::string number = std::to_string(i);
            PhongColoredShader.setVec3("spotLights["+ number + "].position", mSpotLights[i].position);
            PhongColoredShader.setVec3("spotLights["+ number + "].direction", mSpotLights[i].direction);
            PhongColoredShader.setVec3("spotLights["+ number + "].ambient", mSpotLights[i].light->mAmbient);
            PhongColoredShader.setVec3("spotLights["+ number + "].diffuse", mSpotLights[i].light->mDiffuse);
            PhongColoredShader.setVec3("spotLights["+ number + "].specular", mSpotLights[i].light->mSpecular);
            PhongColoredShader.setFloat("spotLights["+ number + "].cutOff", mSpotLights[i].light->mCutOff);
            PhongColoredShader.setFloat("spotLights["+ number + "].outerCutOff", mSpotLights[i].light->mOuterCutOff);
            PhongColoredShader.setFloat("spotLights["+ number + "].constant", mSpotLights[i].light->mConstant);
            PhongColoredShader.setFloat("spotLights["+ number + "].linear", mSpotLights[i].light->mLinear);
            PhongColoredShader.setFloat("spotLights["+ number + "].quadratic", mSpotLights[i].light->mQuadratic);
            PhongColoredShader.setBool("spotLights["+ number + "].on",mSpotLights[i].light->mOn);	
        }
        PhongColoredShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        PhongColoredShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        PhongColoredShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mPhongColoredObjects) {
            // material properties
            PhongColoredShader.setVec3("shapeColor", renderInfo.color);
            PhongColoredShader.setVec3("material.ambient", renderInfo.ke);
            PhongColoredShader.setVec3("material.diffuse", renderInfo.kd);
            PhongColoredShader.setVec3("material.specular", renderInfo.ks); 
            PhongColoredShader.setFloat("material.shininess", 164.0f);
            
            PhongColoredShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(PhongColoredShader);
            
        }   

        // <------ SIMPLE TEXTURED SHADER ------->
        SimpleTexturedShader.use();
        SimpleTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        SimpleTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mSimpleTexturedObjects) {
            SimpleTexturedShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(SimpleTexturedShader);
        }   

        // <------ FLAT TEXTURED SHADER ------->
        FlatTexturedShader.use();
        for (int i = 0; i < mPointLights.size(); i++) {
            std::string number = std::to_string(i);
            FlatTexturedShader.setVec3("pointLights["+ number + "].position", mPointLights[i].first);
            FlatTexturedShader.setVec3("pointLights["+ number + "].ambient", mPointLights[i].second->mAmbient);
            FlatTexturedShader.setVec3("pointLights["+ number + "].diffuse", mPointLights[i].second->mDiffuse);
            FlatTexturedShader.setVec3("pointLights["+ number + "].specular", mPointLights[i].second->mSpecular);
            FlatTexturedShader.setFloat("pointLights["+ number + "].constant", mPointLights[i].second->mConstant);
            FlatTexturedShader.setFloat("pointLights["+ number + "].linear", mPointLights[i].second->mLinear);
            FlatTexturedShader.setFloat("pointLights["+ number + "].quadratic", mPointLights[i].second->mQuadratic);
            FlatTexturedShader.setBool("pointLights["+ number + "].on", mPointLights[i].second->mOn);
        }
        for (int i = 0; i < mSpotLights.size(); i++) {
            std::string number = std::to_string(i);
            FlatTexturedShader.setVec3("spotLights["+ number + "].position", mSpotLights[i].position);
            FlatTexturedShader.setVec3("spotLights["+ number + "].direction", mSpotLights[i].direction);
            FlatTexturedShader.setVec3("spotLights["+ number + "].ambient", mSpotLights[i].light->mAmbient);
            FlatTexturedShader.setVec3("spotLights["+ number + "].diffuse", mSpotLights[i].light->mDiffuse);
            FlatTexturedShader.setVec3("spotLights["+ number + "].specular", mSpotLights[i].light->mSpecular);
            FlatTexturedShader.setFloat("spotLights["+ number + "].cutOff", mSpotLights[i].light->mCutOff);
            FlatTexturedShader.setFloat("spotLights["+ number + "].outerCutOff", mSpotLights[i].light->mOuterCutOff);
            FlatTexturedShader.setFloat("spotLights["+ number + "].constant", mSpotLights[i].light->mConstant);
            FlatTexturedShader.setFloat("spotLights["+ number + "].linear", mSpotLights[i].light->mLinear);
            FlatTexturedShader.setFloat("spotLights["+ number + "].quadratic", mSpotLights[i].light->mQuadratic);
            FlatTexturedShader.setBool("spotLights["+ number + "].on",mSpotLights[i].light->mOn);	
        }
        FlatTexturedShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        FlatTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        FlatTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mFlatTexturedObjects) {
            // material properties
            FlatTexturedShader.setVec3("material.ambient", renderInfo.ke);
            FlatTexturedShader.setVec3("material.diffuse", renderInfo.kd);
            FlatTexturedShader.setVec3("material.specular", renderInfo.ks); 
            FlatTexturedShader.setFloat("material.shininess", 64.0f);
            FlatTexturedShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(FlatTexturedShader);
        }   

        // <------ PHONG TEXTURED SHADER ------->
        PhongTexturedShader.use();
        for (int i = 0; i < mPointLights.size(); i++) {
            std::string number = std::to_string(i);
            PhongTexturedShader.setVec3("pointLights["+ number + "].position", mPointLights[i].first);
            PhongTexturedShader.setVec3("pointLights["+ number + "].ambient", mPointLights[i].second->mAmbient);
            PhongTexturedShader.setVec3("pointLights["+ number + "].diffuse", mPointLights[i].second->mDiffuse);
            PhongTexturedShader.setVec3("pointLights["+ number + "].specular", mPointLights[i].second->mSpecular);
            PhongTexturedShader.setFloat("pointLights["+ number + "].constant", mPointLights[i].second->mConstant);
            PhongTexturedShader.setFloat("pointLights["+ number + "].linear", mPointLights[i].second->mLinear);
            PhongTexturedShader.setFloat("pointLights["+ number + "].quadratic", mPointLights[i].second->mQuadratic);
            PhongTexturedShader.setBool("pointLights["+ number + "].on", mPointLights[i].second->mOn);
        }
        for (int i = 0; i < mSpotLights.size(); i++) {
            std::string number = std::to_string(i);
            PhongTexturedShader.setVec3("spotLights["+ number + "].position", mSpotLights[i].position);
            PhongTexturedShader.setVec3("spotLights["+ number + "].direction", mSpotLights[i].direction);
            PhongTexturedShader.setVec3("spotLights["+ number + "].ambient", mSpotLights[i].light->mAmbient);
            PhongTexturedShader.setVec3("spotLights["+ number + "].diffuse", mSpotLights[i].light->mDiffuse);
            PhongTexturedShader.setVec3("spotLights["+ number + "].specular", mSpotLights[i].light->mSpecular);
            PhongTexturedShader.setFloat("spotLights["+ number + "].cutOff", mSpotLights[i].light->mCutOff);
            PhongTexturedShader.setFloat("spotLights["+ number + "].outerCutOff", mSpotLights[i].light->mOuterCutOff);
            PhongTexturedShader.setFloat("spotLights["+ number + "].constant", mSpotLights[i].light->mConstant);
            PhongTexturedShader.setFloat("spotLights["+ number + "].linear", mSpotLights[i].light->mLinear);
            PhongTexturedShader.setFloat("spotLights["+ number + "].quadratic", mSpotLights[i].light->mQuadratic);
            PhongTexturedShader.setBool("spotLights["+ number + "].on",mSpotLights[i].light->mOn);	
        }
        PhongTexturedShader.setVec3("viewPos", scene.GetCameraHandler().getViewPos());
        PhongTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        PhongTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        for(auto& renderInfo: mPhongTexturedObjects) {
            // material properties
            PhongTexturedShader.setVec3("material.ambient", renderInfo.ke);
            PhongTexturedShader.setVec3("material.diffuse", renderInfo.kd);
            PhongTexturedShader.setVec3("material.specular", renderInfo.ks); 
            PhongTexturedShader.setFloat("material.shininess", 64.0f);
            PhongTexturedShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(PhongTexturedShader);
        }

        SimpleTexturedShader.use();
        SimpleTexturedShader.setMat4("projection", scene.GetCameraHandler().getProjection());
        SimpleTexturedShader.setMat4("view", scene.GetCameraHandler().getViewMatrix());
        // Textures with transparency renderer by order
        while (!mTransparentObjects.empty()) {
            TransparentInfo trparent = mTransparentObjects.top();
            RenderInfo renderInfo = trparent.second;
            SimpleTexturedShader.setMat4("model", renderInfo.modelTransform);
            renderInfo.modelObject->drawCall(SimpleTexturedShader);
            mTransparentObjects.pop();
        }

        if (mSkybox != nullptr){
            mSkybox->Draw(scene.GetCameraHandler().getViewMatrix(), scene.GetCameraHandler().getProjection());
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

    void RenderManager::AddTransparentObjectToRender(float distanceToCam, RenderInfo renderInfo)
    {
        mTransparentObjects.push(std::make_pair(distanceToCam, renderInfo));
    }

    
    void RenderManager::AddPointLightToRender(glm::vec3 pos, PointLightComponent* light)
    {
        mPointLights.push_back(std::make_pair(pos, light));
    }


    void RenderManager::AddSpotLightToRender(glm::vec3 pos, glm::vec3 dir, SpotLightComponent* light) 
    {   
        SpotLightInfo info;
        info.position = pos;
        info.direction = dir;
        info.light = light;
        mSpotLights.push_back(info);
    }
    
    void RenderManager::AddSkyBox(std::vector<std::string> faces)
    {
        mSkybox = new Skybox(faces);
    }

    void RenderManager::CreateDebugObjects()
    {
        CreateDebugFrustrumObject();
        CreateDebugAxisObject();
    }

    void RenderManager::CreateDebugFrustrumObject()
    {
        // Debug Lines
        RenderDebugInfo rInfo;

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
        rInfo.color = glm::vec3(1.0f, 1.0f, 0.0f);
        mDebugObjects.push_back(rInfo);
    }

    RenderDebugInfo createAxisObject(glm::vec3 axis) {
        // Debug Lines
        RenderDebugInfo rInfo;
        unsigned int tmpVBO;
        unsigned int tmpEBO;
        float vertices[] = {
            // positions           
            0.0f, 0.0f,  0.0f,      
            axis.x * 100.0f, axis.y*100.0f, axis.z*100.0f
        };

        unsigned int indices[] = {  // note that we start from 0!
            0, 1
        };

        glGenVertexArrays(1, &rInfo.VAO);
        glGenBuffers(1, &tmpVBO);
        glGenBuffers(1, &tmpEBO);
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
        rInfo.vertexCount = 2;
        rInfo.usingIndices = true;
        rInfo.color = axis;
        rInfo.model = glm::mat4(1.0f);
        return rInfo;
    }

    RenderDebugInfo CreateGridLines(int n, float d)
    {
        RenderDebugInfo rInfo;

        glm::vec2 leftUpCorner = glm::vec2(-n*d, n*d);
        glm::vec2 rightDownCorner = glm::vec2(n*d, -n*d);
        unsigned int lines = 2*n +1;
        std::vector<float> vertices;

        for (unsigned int i = 0; i < lines; i++) 
        {
            float v[6] = {leftUpCorner.x + i*d, leftUpCorner.y, 0.0f,
                          leftUpCorner.x + i*d, rightDownCorner.y, 0.0
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        for (unsigned int j = 0; j < lines; j++)
        {
            float v[6] = {leftUpCorner.x, rightDownCorner.y + j*d, 0.0f,
                          rightDownCorner.x, rightDownCorner.y + j*d, 0.0
            };
            vertices.insert(vertices.end(), std::begin(v), std::end(v));
        }
        float* vert = &vertices[0];
        // first, configure the cube's VAO (and VBO)
        glGenVertexArrays(1, &rInfo.VAO);
        glGenBuffers(1, &rInfo.VBO);

        glBindBuffer(GL_ARRAY_BUFFER, rInfo.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vert, GL_STATIC_DRAW);

        glBindVertexArray(rInfo.VAO);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        rInfo.vertexCount = vertices.size()/3;
        rInfo.usingIndices = false;
        rInfo.color = glm::vec3(0.6f);
        rInfo.model = glm::mat4(1.0f);
        return rInfo;
    }
    
    void RenderManager::CreateDebugAxisObject()
    {
        mDebugObjects.push_back(createAxisObject(glm::vec3(1.0f, 0.0f, 0.0f)));
        mDebugObjects.push_back(createAxisObject(glm::vec3(0.0f, 1.0f, 0.0f)));
        mDebugObjects.push_back(createAxisObject(glm::vec3(0.0f, 0.0f, 1.0f)));

        mDebugObjects.push_back(CreateGridLines(20, 5.0f));
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
        for(std::size_t i = 0; i < mDebugObjects.size(); ++i) {
            RenderDebugInfo renderInfo = mDebugObjects[i];
            shaderProgram.setVec3("shapeColor", renderInfo.color);
            shaderProgram.setMat4("model", renderInfo.model);

            if (i == 0)
                glLineWidth(2.0);
            else if (i == 1)
                glLineWidth(2.0);
            else if (i == 4)
                glLineWidth(1.0);

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

        glLineWidth(1.0);
    }

}