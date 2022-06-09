#include "controlledEntity.hpp"
#include "../../camera/followCamera.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp> 

namespace Journey {

    ControlledEntity::ControlledEntity(std::shared_ptr<FollowCamera> mainCamera) {
        mCamera = mainCamera;
    }

    void ControlledEntity::UserStartUp(Scene& scene)
    {}

    void ControlledEntity::UserUpdate(Scene& scene, float deltaTime)
    {
        mTheta += deltaTime * mRotXVel* mRotSpeed;
        mVel = glm::rotate(mVel, glm::radians(mTheta), mUp);
        mPos += deltaTime * mVel * mRunSpeed;
        if (mPos.z > 0.0f)
            mVel.z -= mGravity * deltaTime;
        else {
            mPos.z = 0.0f;
            mVel.z = 0.0f;
        }
        getTransform().SetTranslation(glm::vec3(mPos.x, 
                                                mPos.y, 
                                                mHeight + mPos.z));
        mAngleZ = glm::orientedAngle(glm::vec2(1.0f, 0.0f), getFixedVel()); 
        //angle = (mVel.x < 0)? -angle:angle;
        getTransform().SetRotation(glm::vec3(0.0f, 0.0f, mAngleZ));

        // Camera 
        auto tPos = getTransform().GetLocalTranslation();
        mCamera->setFollowPoint(glm::vec3(tPos.x, tPos.y, 1.0f));
        mCamera->setFollowRot(mTheta-90.0f);
    }

    glm::vec2 ControlledEntity::getFixedVel() {
        if ( (mVel.x < mDeathZone*3.0f && mVel.x > -mDeathZone*3.0f) && 
             (mVel.y < mDeathZone*3.0f && mVel.y > -mDeathZone*3.0f) ) {
            return glm::normalize(mLastVel);
        }
        else {
            mLastVel = glm::vec2(mVel.x, mVel.y);
        }
        return glm::normalize(glm::vec2(mVel.x, mVel.y));
    }

    void ControlledEntity::moveInX(float dx) {
        mVel.x = (dx > mDeathZone || dx < -mDeathZone) ? dx : 0.0f;
    }

    void ControlledEntity::moveInY(float dy) {
        mVel.y = (dy > mDeathZone || dy < -mDeathZone) ? -dy : 0.0f;
    }

    void ControlledEntity::RotInX(float dx) {
        mRotXVel = (dx > mDeathZone || dx < -mDeathZone) ? -dx : 0.0f;
    }

    void ControlledEntity::jump() {
        if (mPos.z <= 0.0f)
                mVel.z += mJumpForce;
    }
}