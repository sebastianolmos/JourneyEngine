#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>


namespace Journey
{
    class Scene;

    class Window {
        public:
            friend class Scene;
            Window();
            ~Window();

            bool ShouldClose() const;
            glm::ivec2 GetWindowDimensions() const;
            void SetWindowDimensions(const glm::ivec2 &dimensions);

            void UpdatePerformanceMonitor(float deltaTime);

        private:
            void SwapBuffers();
            void StartUp(const std::string title, const int width, const int height);
		    void ShutDown();

            std::string mTitle;
            GLFWwindow* mWindowHandle = nullptr;
            float mTimer;
            float mPeriod;
            int mFramesCounter;
            float mFramesPerSecond;
            float mMsPerFrame;
    };
}