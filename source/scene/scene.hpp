#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "../entity/entity.hpp"
#include "../entity/entityManager.hpp"
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
    class PointLightComponent;


    class Scene {
    
        public:
            friend Engine;
            friend EntityManager;

            InputController& GetInputController();
            RenderManager& GetRenderManager();
            AudioManager& GetAudioManager();
		    Window& GetWindow();
		    void EndApplication();
            void AddEntity(std::shared_ptr<Entity> parentEntity, std::shared_ptr<Entity> newEntity);
            CameraHandler&  GetCameraHandler();

            // Debug methods
            void DebugModeEnabled();
            const bool CanUseDebugMode() const;
            void SetDebugMode(const bool value);
            const bool InDebugMode() const;
            void ToggleDrawMode();

            std::shared_ptr<PointLightComponent> AddPointLightComponent(std::shared_ptr<Entity> entity);

        private:
            Scene(Application& app);
            ~Scene();
            void StartMainLoop();
            void Update(const float deltaTime);
            void deletePointLight();

            InputController mInputController;
            Window mWindow;
            Application& mApplication;
            bool mShouldClose;
            std::shared_ptr<EntityManager> mEntityManager;
            RenderManager mRenderManager;
            AudioManager mAudioManager;
            CameraHandler mCameraHandler;
            bool bAbleToUseDebugMode = false; 
            bool bDebugMode = false; 
    };

}

#endif