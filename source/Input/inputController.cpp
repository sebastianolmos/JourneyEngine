#include "inputController.hpp"
#include <iostream>

namespace Journey {

    void JoystickCallback(int jid, int event) 
    {
        if (event == GLFW_CONNECTED)
        {
            // The joystick was connected
            std::cout << "The joystick " << jid << " was connected" << std::endl;
        }
        else if (event == GLFW_DISCONNECTED)
        {
            // The joystick was disconnected
            std::cout << "The joystick " << jid << " was disconnected" << std::endl;
        }
    }
    
    void InputController::StartUp() 
    {
        glfwSetJoystickCallback(JoystickCallback);
    }

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

    void InputController::BindActionToggle(std::string action, std::function<void(bool)> func)
    {
        if (mKeyMap.count(action)!=0){
            int keyCode = mKeyMap[action];
            if (mOnToggleKeyActions.count(keyCode)==0) {
                std::vector<std::function<void(bool)>> functionVector;
                mOnToggleKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnToggleKeyActions[keyCode].push_back(func);
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
            bool bCurrentState = (currentState==GLFW_PRESS)?true:false;

            // Check if the key status change
            if (key.second != bCurrentState) {
                // Toggle Action
                if (mOnToggleKeyActions.count(key.first)!=0) {
                    for(std::function<void(bool)> func : mOnToggleKeyActions[key.first])  {
                        func(bCurrentState);
                    }
                }
                // Pressed key 
                if (bCurrentState && mOnPressedKeyActions.count(key.first)!=0) {
                    for(std::function<void()> func : mOnPressedKeyActions[key.first])  {
                        func();
                    }
                }
                //Released key
                if (!bCurrentState && mOnReleasedKeyActions.count(key.first)!=0) {
                    for(std::function<void()> func : mOnReleasedKeyActions[key.first])  {
                        func();
                    }
                }
            }        
            key.second = bCurrentState;
        }
    }
}