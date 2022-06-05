#pragma once

#include "camera.hpp"

namespace Journey {
    class FollowCamera : public Camera {
        
    public:

        FollowCamera(const unsigned int screenWidth, const unsigned int screenHeight);

        virtual glm::mat4 getProjection() override;
        virtual glm::mat4 getViewMatrix() override;
        virtual glm::vec3 getViewPos() override;

        void setFollowPoint(glm::vec3 point);
        void setFollowRot(float rot);

        float x = 0.0;
        float y = -0.5;
        float z = 1.2;
        glm::vec3 camDir = glm::vec3(0.0f, 0.6, -0.3);
    
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