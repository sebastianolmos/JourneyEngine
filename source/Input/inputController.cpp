#include "inputController.hpp"
#include "../scene/scene.hpp"

#include <iostream>
#include <assert.h> 

namespace Journey {
    double scrollOffset;

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

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        scrollOffset = yoffset;
    }
    
    void InputController::StartUp(GLFWwindow* window) 
    {
        glfwSetJoystickCallback(JoystickCallback);
        glfwSetScrollCallback(window, scrollCallback);
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

    void InputController::RegisterGamepadAction(std::string actionName, int buttonCode) 
    {
        if (mJoystickMap.count(actionName) == 0 && mJoystickValues.count(buttonCode) == 0){
            mJoystickMap.insert(std::make_pair(actionName, buttonCode));
            mJoystickValues.insert(std::make_pair(buttonCode, false));
        }
        else {
            std::cout << "Action " << actionName << " already registered for Gamepad" << std::endl;
        }
    }

    void InputController::RegisterGamepadAxis(std::string mapName, int axisCode) 
    {
        if (mJoystickAxisMap.count(mapName) == 0){
            mJoystickAxisMap.insert(std::make_pair(mapName, axisCode));
        }
        else {
            std::cout << "Axis " << mapName << " already registered for Gamepad" << std::endl;
        }
    }

    void InputController::BindActionOnPressed(std::string action, std::function<void()> func)
    {   
        const bool bKeyMapHasAction = mKeyMap.count(action)!=0;
        const bool bJoystickMapHasAction = mJoystickMap.count(action)!=0;
        if (!bKeyMapHasAction && !bJoystickMapHasAction) {
            std::cout << "Action " << action << "is not registered" << std::endl;
            return;
        }
        if (bKeyMapHasAction){
            int keyCode = mKeyMap[action];
            if (mOnPressedKeyActions.count(keyCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnPressedKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnPressedKeyActions[keyCode].push_back(func);
        }
        if (bJoystickMapHasAction) {
            int joystickCode = mJoystickMap[action];
            if (mOnPressedJoystickActions.count(joystickCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnPressedJoystickActions.insert(std::make_pair(joystickCode, functionVector));
            }
            mOnPressedJoystickActions[joystickCode].push_back(func);
        }
    }

    void InputController::BindActionOnReleased(std::string action, std::function<void()> func)
    {
        const bool bKeyMapHasAction = mKeyMap.count(action)!=0;
        const bool bJoystickMapHasAction = mJoystickMap.count(action)!=0;
        if (!bKeyMapHasAction && !bJoystickMapHasAction) {
            std::cout << "Action " << action << "is not registered" << std::endl;
            return;
        }
        if (bKeyMapHasAction){
            int keyCode = mKeyMap[action];
            if (mOnReleasedKeyActions.count(keyCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnReleasedKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnReleasedKeyActions[keyCode].push_back(func);
        }
        if (bJoystickMapHasAction) {
            int joystickCode = mJoystickMap[action];
            if (mOnReleasedJoystickActions.count(joystickCode)==0) {
                std::vector<std::function<void()>> functionVector;
                mOnReleasedJoystickActions.insert(std::make_pair(joystickCode, functionVector));
            }
            mOnReleasedJoystickActions[joystickCode].push_back(func);
        }
    }

    void InputController::BindActionToggle(std::string action, std::function<void(bool)> func)
    {
        const bool bKeyMapHasAction = mKeyMap.count(action)!=0;
        const bool bJoystickMapHasAction = mJoystickMap.count(action)!=0;
        if (!bKeyMapHasAction && !bJoystickMapHasAction) {
            std::cout << "Action " << action << "is not registered" << std::endl;
            return;
        }
        if (bKeyMapHasAction){
            int keyCode = mKeyMap[action];
            if (mOnToggleKeyActions.count(keyCode)==0) {
                std::vector<std::function<void(bool)>> functionVector;
                mOnToggleKeyActions.insert(std::make_pair(keyCode, functionVector));
            }
            mOnToggleKeyActions[keyCode].push_back(func);
        }
        if (bJoystickMapHasAction) {
            int joystickCode = mJoystickMap[action];
            if (mOnToggleJoystickActions.count(joystickCode)==0) {
                std::vector<std::function<void(bool)>> functionVector;
                mOnToggleJoystickActions.insert(std::make_pair(joystickCode, functionVector));
            }
            mOnToggleJoystickActions[joystickCode].push_back(func);
        }
    }

    void InputController::BindAxisMap(std::string mapName, std::function<void(float)> func)
    {
        if (mJoystickAxisMap.count(mapName)!=0) {
            int joystickCode = mJoystickAxisMap[mapName];
            if (mOnJoystickAxis.count(joystickCode)==0) {
                std::vector<std::function<void(float)>> functionVector;
                mOnJoystickAxis.insert(std::make_pair(joystickCode, functionVector));
            }
            mOnJoystickAxis[joystickCode].push_back(func);
        }
        else {
            std::cout << "Axis " << mapName << "is not registered for GamePad" << std::endl;
        }
    }

    void InputController::CheckIfDebug(GLFWwindow* window, Scene& scene) {
        bool bCurrentState = (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS &&
                              glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS)?true:false;

        if (!debugKeyPressed && bCurrentState) {
            scene.SetDebugMode(!scene.InDebugMode());
            std::cout << "Debug Mode " << (scene.InDebugMode()?"ON":"OFF") << std::endl;

            if (scene.InDebugMode()) {
                mLastCamera = scene.GetCameraHandler().getCurrentCamera();
                scene.GetCameraHandler().setCurrentCamera(mDebugCam);
            }
            else {
                scene.GetCameraHandler().setCurrentCamera(mLastCamera);
            }
        }

        debugKeyPressed = bCurrentState;
    }

    void InputController::PollDevices(GLFWwindow* window, Scene& scene) {
        glfwPollEvents();
        if (scene.CanUseDebugMode())
            CheckIfDebug(window, scene);

        if (scene.InDebugMode())
            PollOnDebug(window);
        else {
            PollKeyboard(window);
            PollJoystick();
        }
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
            return;
        }

        int buttonsCount;
        const unsigned char* buttons = glfwGetJoystickButtons(joystickId, &buttonsCount);
        // Loop the Joystick buttons
        for (auto& jButton: mJoystickValues) {
            if (jButton.first >= buttonsCount)
                continue;
            int currentState = buttons[jButton.first];
            bool bCurrentState = (currentState==GLFW_PRESS)?true:false;
            
            // Check if the J button status change
            if (jButton.second != bCurrentState) {
                
                // Toggle Action
                if (mOnToggleJoystickActions.count(jButton.first)!=0) {
                    for(std::function<void(bool)> func : mOnToggleKeyActions[jButton.first])
                        func(bCurrentState);
                }
                // Pressed key 
                if (bCurrentState && mOnPressedJoystickActions.count(jButton.first)!=0) {
                    for(std::function<void()> func : mOnPressedJoystickActions[jButton.first])  
                        func();
                }
                //Released key
                 if (!bCurrentState && mOnReleasedJoystickActions.count(jButton.first)!=0) {
                    for(std::function<void()> func : mOnReleasedJoystickActions[jButton.first])  
                        func();
                }
            }
            jButton.second = bCurrentState;
        }

        int axesCount;
        const float* axes = glfwGetJoystickAxes(joystickId, &axesCount);
        // Loop the Joystick Axis
        for (auto& jAxis: mOnJoystickAxis) {
            if (jAxis.first >= axesCount)
                continue;
            float currentValue = axes[jAxis.first];
            for(std::function<void(float)> func : mOnJoystickAxis[jAxis.first]) 
                func(currentValue);
        }
    }

    void InputController::PollOnDebug(GLFWwindow* window)
    {
        if (mDebugCam == nullptr)
            return;

        // Mouse buttons
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            mDebugCam->SetRotDrag(true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            mDebugCam->SetRotDrag(false);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            mDebugCam->SetCenterDrag(true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
            mDebugCam->SetCenterDrag(false);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            mDebugCam->SetVerticalDrag(true);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
            mDebugCam->SetVerticalDrag(false);

        // Mouse Position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mDebugCam->SetCurrentMousePos(xpos, ypos);

        // Mouse Scroll
        mDebugCam->ProcessMouseScroll(scrollOffset);
        scrollOffset = 0.0f;
    }

    void InputController::StartDebugCamera(const unsigned int screenWidth, const unsigned int screenHeight) 
    {
        mDebugCam = std::make_shared<DebugCamera>(screenWidth, screenHeight);
    }
}