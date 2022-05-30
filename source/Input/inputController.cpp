#include "inputController.hpp"
#include <iostream>

namespace Journey {
    void InputController::RegisterKeyAction(std::string actionName, int keyCode)
    {   
        if (mKeyMap.count(actionName) == 0 && mKeyValues.count(keyCode) == 0){
            mKeyMap.insert(std::make_pair(actionName, keyCode));
            mKeyValues.insert(std::make_pair(keyCode, false));
        }
        else {
            std::cout << "Action " << actionName << " already registered" << std::endl;
        }
    }

    void InputController::BindActionOnPressed(std::string action, std::function<void()> func)
    {
        if (mKeyMap.count(action)!=0){
            int keyCode = mKeyMap[action];
            if (mOnPressedKeyActions.count(keyCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnPressedKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnPressedKeyActions[keyCode].push_back(func);
        }
        else {
            std::cout << "Action " << action << "is not registered" << std::endl;
        }
    }

    void InputController::BindActionOnReleased(std::string action, std::function<void()> func)
    {
        if (mKeyMap.count(action)!=0){
            int keyCode = mKeyMap[action];
            if (mOnReleasedKeyActions.count(keyCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnReleasedKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnReleasedKeyActions[keyCode].push_back(func);
        }
        else {
            std::cout << "Action " << action << "is not registered" << std::endl;
        }
    }

    void InputController::PollDevices(GLFWwindow* window) {
        PollKeyboard(window);
    }

    void InputController::PollKeyboard(GLFWwindow* window) {
        for (auto& key: mKeyValues) {
            int currentState = glfwGetKey(window, key.first);
            // Pressed key
            if (!key.second && currentState && mOnPressedKeyActions.count(key.first)!=0)
            {
                for(std::function<void()> func : mOnPressedKeyActions[key.first])  {
                    func();
                }
            }

            // Released Key
            if (key.second && !currentState && mOnReleasedKeyActions.count(key.first)!=0)
            {
                for(std::function<void()> func : mOnReleasedKeyActions[key.first])  {
                    func();
                }
            }

            key.second = currentState;
        }
    }
}