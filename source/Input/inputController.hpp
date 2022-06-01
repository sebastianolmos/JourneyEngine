#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

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
    };

    class InputController
    {
    public:
        friend class Scene;
        InputController() = default;
        
        void RegisterKeyAction(std::string actionName, int keyCode);

        void BindActionOnPressed(std::string action, std::function<void()> func);
        void BindActionOnReleased(std::string action, std::function<void()> func);
        void BindActionToggle(std::string action, std::function<void(bool)> func);

    private:
        void StartUp();
        void PollDevices(GLFWwindow* window);

        void PollKeyboard(GLFWwindow* window);
        void PollJoystick();

        std::unordered_map<std::string, int> mKeyMap;
        std::unordered_map<int, bool> mKeyValues;

        std::unordered_map<int, std::vector<std::function<void()>> > mOnPressedKeyActions;
        std::unordered_map<int, std::vector<std::function<void()>> > mOnReleasedKeyActions;
        std::unordered_map<int, std::vector<std::function<void(bool)>> > mOnToggleKeyActions;

    };

}