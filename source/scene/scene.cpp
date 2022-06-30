#include "scene.hpp"
#include <chrono>

namespace Journey {

    Scene::Scene(Application& app) :
        mInputController(),
        mWindow(),
        mApplication(app),
        mShouldClose(false),
        mRenderManager()
    {   
        mEntityManager = std::make_shared<EntityManager>(*this);
        mWindow.StartUp(mApplication.GetName(), mApplication.GetWindowWith(), mApplication.GetWindowHeight());
        mInputController.StartUp(mWindow.mWindowHandle);
        mEntityManager->StartUp();
        mRenderManager.StartUp();
        mAudioManager.StartUp();

        mApplication.StartUp(*this);
    }

    Scene::~Scene()
    {
        mApplication.UserShutDown(*this);

        mRenderManager.ShutDown();
        mEntityManager->ShutDown();
        mAudioManager.ShutDown();
		// ShotDown the Input Controller
        mWindow.ShutDown();
    }

    void Scene::StartMainLoop()
    {
        std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
        while (!mWindow.ShouldClose() && !mShouldClose)
		{
			std::chrono::time_point<std::chrono::steady_clock> newTime = std::chrono::steady_clock::now();
			const auto frameTime = newTime - startTime;
			startTime = newTime;
			float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(frameTime).count();
			Update(deltaTime);
		}

        mWindow.ShouldClose();
    }

    void Scene::Update(const float deltaTime)
    {   
        mWindow.UpdatePerformanceMonitor(deltaTime);
        mInputController.PollDevices(mWindow.mWindowHandle, *this, deltaTime);
        if (!InDebugMode()) {
            mRenderManager.CleanRenderInfo();
            mAudioManager.cleanBatch();
            mEntityManager->UpdateEntities(*this, deltaTime);
            mApplication.UserUpdate(*this, deltaTime);
            mEntityManager->DeleteEntities();
        }
        mAudioManager.updateListener(*this, deltaTime);
        mAudioManager.updateSources(deltaTime);
        mRenderManager.DrawCall(*this);
        mWindow.SwapBuffers();
    }

    InputController& Scene::GetInputController()
    {
        return mInputController;
    }

	Window& Scene::GetWindow()
    {   
        return mWindow;
    }   

	void Scene::EndApplication()
    {
        mShouldClose = true;
    }

    RenderManager& Scene::GetRenderManager()
    {
        return mRenderManager;
    }

    CameraHandler&  Scene::GetCameraHandler()
    {
        return mCameraHandler;
    }

    PointLight&  Scene::GetPointLight()
    {
        return mPointLight;
    }

    void Scene::AddEntity(std::shared_ptr<Entity> parentEntity, std::shared_ptr<Entity> newEntity)
    {
        mEntityManager->AddEntity(parentEntity, newEntity);
    }

    void Scene::DebugModeEnabled()
    {
        bAbleToUseDebugMode = true;
        mInputController.StartDebugCamera(mApplication.mWidth, mApplication.mHeight);
        mRenderManager.CreateDebugObjects();
    }

    const bool Scene::CanUseDebugMode() const 
    {
        return bAbleToUseDebugMode;
    }

    void Scene::SetDebugMode(const bool value)
    {
        bDebugMode = value;
    }
    
    const bool Scene::InDebugMode() const
    {
        return bDebugMode;
    }

    void Scene::ToggleDrawMode()
    {
        mRenderManager.mDrawLines = !mRenderManager.mDrawLines;
    }

     AudioManager& Scene::GetAudioManager()
    {
        return mAudioManager;
    }

}
