#pragma once

#include "../window/window.hpp"
#include "../input/inputController.hpp"
#include "../application.hpp"
#include <string>

namespace Journey {
    class Engine;
    class Scene {
    
        public:
            friend Engine;

            InputController& GetInputController();
		    Window& GetWindow();
		    void EndApplication();

        private:
            Scene(Application& app);
            ~Scene();
            void StartMainLoop();
            void Update(const float deltaTime);

            InputController mInputController;
            Window mWindow;
            Application& mApplication;
            bool mShouldClose;
    };

}