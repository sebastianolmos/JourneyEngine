#include "debugCamera.hpp"

namespace Journey {

    DebugCamera::DebugCamera(const unsigned int screenWidth, const unsigned int screenHeight)
    {
        mScrWidth = screenWidth;
        mScrHeight = screenHeight;
    }

    glm::mat4 DebugCamera::getProjection()
    {
        return glm::perspective(glm::radians(mFov), (float)mScrWidth / (float)mScrHeight, mNear, mFar);
    }

    glm::mat4 DebugCamera::getViewMatrix()
    {
        mPosition.x = mCenter.x + mRadius * cos(glm::radians(mPhi)) * sin(glm::radians(mTheta));
        mPosition.y = mCenter.y + mRadius * sin(glm::radians(mPhi)) * sin(glm::radians(mTheta));
        mPosition.z = mCenter.z + mRadius * cos(glm::radians(mTheta));
        return glm::lookAt(mPosition, mCenter, mWorldUp);
    }

    glm::vec3 DebugCamera::getViewPos()
    {
        return mPosition;
    }

    void DebugCamera::SetRotDrag(bool value)
    {
        mRotDrag = value;
    }

    void DebugCamera::SetCenterDrag(bool value)
    {
        mCenterDrag = value;
    }

    void DebugCamera::SetVerticalDrag(bool value)
    {
        mVertDrag = value;
    }

    void DebugCamera::SetCurrentMousePos(float xPos, float yPos)
    {   
        float posX = 2 * (xPos - mScrWidth / 2) / mScrWidth;
        float posY = 2 * (mScrHeight / 2 - yPos) / mScrHeight;
        glm::vec2 pos2d{ posX, posY };

        if (mRotDrag)
        {
            glm::vec2 delta = (pos2d - mCurrentMousePos);
            mPhi -= delta.x * mRotSensitivity;
            mTheta += delta.y * mRotSensitivity;
        }
        if (mTheta > 179.0f)
            mTheta = 179.0f;
        if (mTheta < 01.0f)
            mTheta = 01.0f;

        if (mCenterDrag)
        {
            glm::vec2 delta = (pos2d - mCurrentMousePos) * -1.0f;
            
            mCenter += mMovRight * delta.x * mCenterSensitivty * mRadius / 4.0f
                + mMovFront * delta.y * mCenterSensitivty * mRadius / 4.0f;
        }
        else if (mVertDrag)
        {
            glm::vec2 delta = (pos2d - mCurrentMousePos) * -1.0f;
            mCenter += mUp * delta.y * mCenterSensitivty * mRadius / 4.0f;
        }
        mCurrentMousePos.x = posX;
        mCurrentMousePos.y = posY;
        updateCameraVectors();
    }

    void DebugCamera::ProcessMouseScroll(float yoffset)
    {
        mRadius -= (float)yoffset * 0.2f;
        if (mRadius < 1.0f)
            mRadius = 1.0f;
        if (mRadius > 100.0f)
            mRadius = 100.0f;
    }

    void DebugCamera::ProcessKeyboardMovement(ECameraMovement direction, float deltaTime)
    {
        float velocity = mMovementSpeed * deltaTime;
        switch (direction)
        {
            case ECameraMovement::FORWARD:
                mCenter += mMovFront * velocity;
                break;
            case ECameraMovement::BACKWARD:
                mCenter -= mMovFront * velocity;
                break;
            case ECameraMovement::LEFT:
                mCenter -= mMovRight * velocity;
                break;
            case ECameraMovement::RIGHT:
                mCenter += mMovRight * velocity;
                break;
            case ECameraMovement::ORIGIN:
                mCenter = glm::vec3(0.0f);
                break;
            default:
                break;
        }
    }
    
    void DebugCamera::ProcessKeyboardRotation(ECameraRotation direction, float deltaTime)
    {
        float velocity = mRotationSpeed * deltaTime;

        switch (direction)
        {
            case ECameraRotation::AZIM_UP:
                mTheta -= velocity;
                break;
            case ECameraRotation::AZIM_DOWN:
                mTheta += velocity;
                break;
            case ECameraRotation::ZEN_LEFT:
                mPhi -= velocity;
                break;
            case ECameraRotation::ZEN_RIGHT:
                mPhi += velocity;
                break;
            default:
                break;
        }
        if (mTheta > 179.0f)
            mTheta = 179.0f;
        if (mTheta < 01.0f)
            mTheta = 01.0f;

        updateCameraVectors();
    }

    void DebugCamera::updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front = glm::normalize(mCenter - mPosition);
        mFront = glm::vec3(front.x, front.y, front.z);
        //mFront = glm::normalize(mFront);
        // also re-calculate the Right and Up vector
        mMovRight = glm::cross(mFront, mWorldUp);
        mMovFront = glm::cross(mWorldUp, mMovRight);// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        mUp = glm::cross(mMovRight, mFront);
        mRight = glm::cross(mFront, mUp);
    }
}