#pragma once

#include "camera.hpp"

namespace Journey {

    enum ECameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        ORIGIN
    };

    enum ECameraRotation {
        AZIM_UP,
        AZIM_DOWN,
        ZEN_LEFT,
        ZEN_RIGHT
    };

    class DebugCamera : public Camera {
        
    public:
        DebugCamera(const unsigned int screenWidth, const unsigned int screenHeight);

        virtual glm::mat4 getProjection() override;

        virtual glm::mat4 getViewMatrix() override;

        virtual glm::vec3 getViewPos() override;

        virtual glm::vec3 getFront() override;

        virtual glm::vec3 getUp() override;

        void SetRotDrag(bool value);
        void SetCenterDrag(bool value);
        void SetVerticalDrag(bool value);
        void SetCurrentMousePos(float xPos, float yPos);
        void ProcessMouseScroll(float yoffset);
        void ProcessKeyboardMovement(ECameraMovement direction, float deltaTime);
        void ProcessKeyboardRotation(ECameraRotation direction, float deltaTime);

    private:
        unsigned int mScrWidth;
        unsigned int mScrHeight;

        glm::vec3 mPosition = glm::vec3(1.0f);
        glm::vec3 mCenter = glm::vec3(0.0f);
        glm::vec3 mWorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 mFront = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 mUp = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 mRight = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 mMovFront = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 mMovRight = glm::vec3(0.0f, 1.0f, 0.0f);
        float mRadius = 3.0f;
        float mPhi = 0.0f;
        float mTheta = 50.0f;
        float mFov = 60.0f;
        float mNear = 0.1f;
        float mFar = 1500.0f;
        bool mCenterDrag = false;
        bool mRotDrag = false;
        bool mVertDrag = false;
        float mRotSensitivity = 55.0f;
        float mCenterSensitivty = 8.0f;
        float mMovementSpeed = 15.0f;
        float mRotationSpeed = 75.0f;
        glm::vec2 mCurrentMousePos = glm::vec2(0.0f);

        void updateCameraVectors();
    };
}