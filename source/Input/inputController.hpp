#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

namespace Journey {

    class InputController
    {
    public:
        InputController() = default;
        
        void RegisterKeyAction(std::string actionName, int keyCode);

        void BindActionOnPressed(std::string action, std::function<void()> func);
        void BindActionOnReleased(std::string action, std::function<void()> func);
        void BindActionToggle(std::string action, std::function<void(bool)> func);

        void PollDevices(GLFWwindow* window);
    private:

        void PollKeyboard(GLFWwindow* window);

        std::unordered_map<std::string, int> mKeyMap;
        std::unordered_map<int, bool> mKeyValues;

        std::unordered_map<int, std::vector<std::function<void()>> > mOnPressedKeyActions;
        std::unordered_map<int, std::vector<std::function<void()>> > mOnReleasedKeyActions;
        std::unordered_map<int, std::vector<std::function<void(bool)>> > mOnToggleKeyActions;

    };

}