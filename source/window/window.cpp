#include "window.hpp"
#include <glad/glad.h>

namespace Journey {

    Window::Window() = default;

	Window::~Window() = default;
    
    void Window::StartUp(const std::string title, const int width, const int height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
        // glfw window creation
    // --------------------
        std::string appTitle = "Journey Engine - " + title;
        mTitle = appTitle;
        mWindowHandle = glfwCreateWindow(width, height, appTitle.c_str(), NULL, NULL);
        if (mWindowHandle == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(mWindowHandle);
        glfwSetFramebufferSizeCallback(mWindowHandle, [](GLFWwindow* window, int width, int height)
            {
					glViewport(0, 0, width, height);
            }
        );
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        mTimer = 0.0f;
        mPeriod = 0.5f;
        mFramesCounter = 0;
        mFramesPerSecond = 0.0f;
        mMsPerFrame = 0.0f;
    }
	
    void Window::ShutDown()
    {
        glfwDestroyWindow(mWindowHandle);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(mWindowHandle);
        glfwPollEvents();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(mWindowHandle);
    }
    
    glm::ivec2 Window::GetWindowDimensions() const
    {
        int width, height;
		glfwGetFramebufferSize(mWindowHandle, &width, &height);
		return glm::ivec2(width, height);
    }
    
     void Window::SetWindowDimensions(const glm::ivec2 &dimensions)
     {
         glfwSetWindowSize(mWindowHandle, dimensions.x, dimensions.y);
     }

     void Window::UpdatePerformanceMonitor(float deltaTime)
     {
        mFramesCounter += 1;
        mTimer += deltaTime;

        if (mTimer > mPeriod)
        {
            mFramesPerSecond = mFramesCounter / mTimer;
            mMsPerFrame = 1000.0 * mTimer / mFramesCounter;
            mFramesCounter = 0;
            mTimer = 0.0f;
        }

        std::stringstream ss;
        ss << mTitle << " " << std::fixed << std::setprecision(2)
		<< "[" << mFramesPerSecond << " fps - "
		<< mMsPerFrame << " ms]";

        glfwSetWindowTitle(mWindowHandle, ss.str().c_str());
     }

}