#pragma once

#include <string>

namespace Journey {

    class Window;
    class InputController;
    class Scene;
    class Application
    {
        public:
            friend class Scene;

            virtual void UserStartUp(Scene& scene) = 0;
            virtual void UserShutDown(Scene& scene) = 0;
            virtual void UserUpdate(Scene& scene, float deltaTime) = 0;
            virtual ~Application() = default;

            const std::string& GetName();
            int GetWindowWith();
            int GetWindowHeight();

        protected:
            std::string mName = "Game title";
            int mWidth = 800;
            int mHeight = 600; 
        
        private:
            void StartUp(Scene& scene);
    };

}