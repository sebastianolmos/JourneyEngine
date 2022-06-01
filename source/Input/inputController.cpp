#include "inputController.hpp"
#include <iostream>
#include <assert.h> 

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
        PollJoystick();
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

    void InputController::PollJoystick() {
        int joystickId = GLFW_JOYSTICK_1;
        int const joystickConnected = glfwJoystickPresent(joystickId);

        if (joystickConnected == GLFW_FALSE) {
            if (mGamePad!=nullptr) 
                delete(mGamePad);
            return;
        }
        
        // At this point we have a joystick with this id.
        if (mGamePad==nullptr)
        {
            /* joystickId has been connected.*/
            int buttonsCount;
            glfwGetJoystickButtons(joystickId, &buttonsCount);

            int axesCount;
            glfwGetJoystickAxes(joystickId, &axesCount);

            mGamePad = new Joystick(buttonsCount, axesCount);
        }

        int buttonsCount;
        const unsigned char* buttons = glfwGetJoystickButtons(joystickId, &buttonsCount);
        assert(mGamePad->buttons.size() == buttonsCount);

        for (int buttonId = 0; buttonId < buttonsCount; ++buttonId)
        {
            mGamePad->buttons[buttonId] = buttons[buttonId] == GLFW_PRESS;
            if (mGamePad->buttons[buttonId]){
                std::cout << "GamePad Button Pressed:  " << buttonId << std::endl;
            }
        }

        int axesCount;
        const float* axes = glfwGetJoystickAxes(joystickId, &axesCount);
        assert(mGamePad->axes.size() == axesCount);

        for (int axesId = 0; axesId < axesCount; ++axesId)
        {
            mGamePad->axes[axesId] = axes[axesId];
            if (mGamePad->axes[axesId] > 0.5){
                std::cout << "GamePad Axis Moved:  " << axesId << std::endl;
            }
        }
    }
}