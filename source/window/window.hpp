#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

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

        private:
            void SwapBuffers();
            void StartUp(const std::string title, const int width, const int height);
		    void ShutDown();

            GLFWwindow* mWindowHandle = nullptr;
    };
}