#include "scene.hpp"
#include <chrono>

namespace Journey {

    Scene::Scene(Application& app) :
        //mInputController(),
        mWindow(),
        mApplication(app),
        mShouldClose(false)
    {
        mWindow.StartUp(mApplication.GetName(), mApplication.GetWindowWith(), mApplication.GetWindowHeight());
        // InputController stratUp

        mApplication = std::move(app);
        mApplication.StartUp(*this);
    }

    Scene::~Scene()
    {
        mApplication.UserShutDown(*this);

        mWindow.ShutDown();
		// ShotDown the Input Controller
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
        //Update input
        mApplication.UserUpdate(*this, deltaTime);
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

}
