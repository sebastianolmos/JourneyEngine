#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../camera/cameraHandler.hpp"
#include "../camera/debugCamera.hpp"
#include "../camera/followCamera.hpp"

namespace Journey {
    class Scene;
    // A simple struct handling all axes and button inputs of a joystick. 
    struct Joystick
    {
        std::vector<bool> buttons;
        std::vector<float> axes;

        Joystick(std::size_t const numberOfButtons, std::size_t const numberOfAxes):
            buttons(numberOfButtons, false), axes(numberOfAxes, 0.0f)
        {}

        ~Joystick() {
            buttons.clear();
            axes.clear();
        }
    };

    class InputController
    {
    public:
        friend class Scene;
        InputController() = default;
        
        void RegisterKeyAction(std::string actionName, int keyCode);
        void RegisterGamepadAction(std::string actionName, int buttonCode);
        void RegisterGamepadAxis(std::string mapName, int axisCode);

        void BindActionOnPressed(std::string action, std::function<void()> func);
        void BindActionOnReleased(std::string action, std::function<void()> func);
        void BindActionToggle(std::string action, std::function<void(bool)> func);
        void BindAxisMap(std::string mapName, std::function<void(float)> func);

        void StartDebugCamera(const unsigned int screenWidth, const unsigned int screenHeight);
        std::shared_ptr<Camera> GetLastCamera();

    private:
        void StartUp(GLFWwindow* window);
        void PollDevices(GLFWwindow* window, Scene& scene, float deltaTime);

        void PollKeyboard(GLFWwindow* window);
        void PollJoystick();

        void PollOnDebug(GLFWwindow* window, Scene& scene, float deltaTime);

        void CheckIfDebug(GLFWwindow* window, Scene& scene);
        bool debugKeyPressed = false;

        // Keyboard
        std::unordered_map<std::string, int> mKeyMap;
        std::unordered_map<int, bool> mKeyValues;

        std::unordered_map<int, std::vector<std::function<void()>> > mOnPressedKeyActions;
        std::unordered_map<int, std::vector<std::function<void()>> > mOnReleasedKeyActions;
        std::unordered_map<int, std::vector<std::function<void(bool)>> > mOnToggleKeyActions;

        // Joystick
        Joystick* mGamePad;
        std::unordered_map<std::string, int> mJoystickMap;
        std::unordered_map<int, bool> mJoystickValues;

        std::unordered_map<int, std::vector<std::function<void()>> > mOnPressedJoystickActions;
        std::unordered_map<int, std::vector<std::function<void()>> > mOnReleasedJoystickActions;
        std::unordered_map<int, std::vector<std::function<void(bool)>> > mOnToggleJoystickActions;

        std::unordered_map<std::string, int> mJoystickAxisMap;
        std::unordered_map<int, std::vector<std::function<void(float)>> > mOnJoystickAxis;

        //Debug vars
        std::shared_ptr<Camera> mLastCamera;
        std::shared_ptr<DebugCamera> mDebugCam;
        bool mDebugLineMode = false;
    };

}