#pragma once

#include "scene/scene.hpp"
#include "application.hpp"

namespace Journey {

    class Engine 
    {
        public:
            Engine(Application& app) : mScene(app) {}
            ~Engine() = default;

            void StartMainLoop() 
            {
                mScene.StartMainLoop();
            }

        private:
            Scene mScene;
    };

}