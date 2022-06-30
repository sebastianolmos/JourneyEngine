#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "../entity/entity.hpp"
#include "../entity/entityManager.hpp"
#include "../entity/pointLight.hpp"
#include "../camera/cameraHandler.hpp"
#include "../component/transform.hpp"
#include "../component/component.hpp"
#include "../component/staticMesh.hpp"
#include "../component/sprite.hpp"
#include "../rendering/material.hpp"
#include "../rendering/renderManager.hpp"
#include "../audio/audioManager.hpp"
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
            CameraHandler&  GetCameraHandler();
            PointLight&  GetPointLight();

            // Debug methods
            void DebugModeEnabled();
            const bool CanUseDebugMode() const;
            void SetDebugMode(const bool value);
            const bool InDebugMode() const;
            void ToggleDrawMode();

        private:
            Scene(Application& app);
            ~Scene();
            void StartMainLoop();
            void Update(const float deltaTime);

            InputController mInputController;
            Window mWindow;
            Application& mApplication;
            bool mShouldClose;
            std::shared_ptr<EntityManager> mEntityManager;
            RenderManager mRenderManager;
            AudioManager mAudioManager;
            CameraHandler mCameraHandler;
            PointLight mPointLight;
            bool bAbleToUseDebugMode = false; 
            bool bDebugMode = false; 
    };

}

#endif