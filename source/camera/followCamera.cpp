#include "followCamera.hpp"

namespace Journey {

    FollowCamera::FollowCamera(const unsigned int screenWidth, const unsigned int screenHeight)
    {
        mScrWidth = screenWidth;
        mScrHeight = screenHeight;
    }

    glm::mat4 FollowCamera::getProjection()
    {
        return glm::perspective(glm::radians(mFov), (float)mScrWidth / (float)mScrHeight, mNear, mFar);
    }

    glm::mat4 FollowCamera::getViewMatrix()
    {
        mCenter = mFollowPoint + mWorldUp * mCenterHeight;
        mPosition.x = mCenter.x + mRadius * cos(glm::radians(mPhi)) * sin(glm::radians(mTheta));
        mPosition.y = mCenter.y + mRadius * sin(glm::radians(mPhi)) * sin(glm::radians(mTheta));
        mPosition.z = mCenter.z + mRadius * cos(glm::radians(mTheta));
        return glm::lookAt(mPosition, mCenter, mWorldUp);
    }

    glm::vec3 FollowCamera::getViewPos()
    {
        return  mPosition;
    }

    void FollowCamera::setFollowPoint(glm::vec3 point)
    {
        mFollowPoint = point;
    }
    
    void FollowCamera::setFollowRot(float rot)
    {
        mPhi = rot;
    }

    void FollowCamera::setCenterHeight(float value)
    {
        mCenterHeight = value;
    }

    void FollowCamera::setDistanceRadius(float value)
    {
        mRadius = value;
    }   

    void FollowCamera::setFov(float value)
    {
        mFov = value;
    }

    void FollowCamera::setNear(float value)
    {
        mNear = value;
    }

    void FollowCamera::setFar(float value)
    {
        mFar = value;
    }

    void FollowCamera::setThetaAngle(float value)
    {
        mTheta = value;
    }

    glm::vec3 FollowCamera::getFront()
    {
        return mCenter-mPosition;
    }

    glm::vec3 FollowCamera::getUp()
    {
        return mWorldUp;
    }
}