#pragma once

#include "camera.hpp"

namespace Journey {
    class FollowCamera : public Camera {
        
    public:

        FollowCamera(const unsigned int screenWidth, const unsigned int screenHeight);

        virtual glm::mat4 getProjection() override;
        virtual glm::mat4 getViewMatrix() override;
        virtual glm::vec3 getViewPos() override;
        virtual glm::vec3 getFront() override;
        virtual glm::vec3 getUp() override;

        void setFollowPoint(glm::vec3 point);
        void setFollowRot(float rot);
        void setCenterHeight(float value);
        void setDistanceRadius(float value);
        void setFov(float value);
        void setNear(float value);
        void setFar(float value);
        void setThetaAngle(float value);
    
    private:
        unsigned int mScrWidth;
        unsigned int mScrHeight;

        glm::vec3 mPosition = glm::vec3(1.0f);
        glm::vec3 mCenter = glm::vec3(0.0f);
        glm::vec3 mWorldUp = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 mFollowPoint = glm::vec3(0.0f);
        float mCenterHeight = 1.0f;
        float mRadius = 3.0f;
        float mFov = 60.0f;
        float mNear = 2.0f;
        float mFar = 30.0f;
        float mPhi = 0.0f;
        float mTheta = 77.0f;
    };
}