#pragma once

#include "../entity/entity.hpp"
#include "../entity/entityManager.hpp"
#include "../entity/cameraHandler.hpp"
#include "../entity/pointLight.hpp"
#include "../component/transform.hpp"
#include "../component/component.hpp"
#include "../component/staticMesh.hpp"
#include "../component/sprite.hpp"
#include "../rendering/material.hpp"
#include "../rendering/meshManager.hpp"
#include "../rendering/textureManager.hpp"
#include "../rendering/renderManager.hpp"
#include "../window/window.hpp"
#include "../input/inputController.hpp"
#include "../application.hpp"

#include <memory>
#include <string>

namespace Journey {

    class Engine;
    class Scene {
    
        public:
            friend Engine;

            InputController& GetInputController();
            RenderManager& GetRenderManager();
		    Window& GetWindow();
		    void EndApplication();
            void AddEntity(std::shared_ptr<Entity> parentEntity, std::shared_ptr<Entity> newEntity);
            void AddPrimitiveMeshComponent(std::shared_ptr<Entity> entity, std::shared_ptr<Material> material, EPrimitiveMesh primitiveMesh);
            
            void AddSpriteComponent(std::shared_ptr<Entity> entity,  std::shared_ptr<Material> material, std::string spritePath);

            CameraHandler&  GetCameraHandler();
            PointLight&  GetPointLight();

        private:
            Scene(Application& app);
            ~Scene();
            void StartMainLoop();
            void Update(const float deltaTime);

            InputController mInputController;
            Window mWindow;
            Application& mApplication;
            bool mShouldClose;
            EntityManager mEntityManager;
            RenderManager mRenderManager;
            TextureManager mTextureManager;
            MeshManager mMeshManager;
            CameraHandler mCameraHandler;
            PointLight mPointLight;

    };

}