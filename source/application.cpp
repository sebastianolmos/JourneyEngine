#include "application.hpp"

namespace Journey {
    void Application::StartUp(Scene& scene)
    {
        UserStartUp(scene);
    }

    const std::string& Application::GetName(){
        return mName;
    }

    int Application::GetWindowWith()
    {
        return mWidth;
    }

    int Application::GetWindowHeight()
    {
        return mHeight;
    }

}