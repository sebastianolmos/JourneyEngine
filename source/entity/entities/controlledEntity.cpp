#include "controlledEntity.hpp"
#include "../../camera/followCamera.hpp"
#include <glm/gtx/rotate_vector.hpp>

namespace Journey {

    ControlledEntity::ControlledEntity(std::shared_ptr<FollowCamera> mainCamera) {
        mCamera = mainCamera;
    }

    void ControlledEntity::UserStartUp(Scene& scene)
    {}

    void ControlledEntity::UserUpdate(Scene& scene, float deltaTime)
    {
        mTheta += deltaTime * mRotXVel* 50.0f;
        glm::vec3 finalVel = glm::rotate(mVel, glm::radians(mTheta), mUp);
        mPos += deltaTime * finalVel * 3.0f;
        if (mPos.z > 0.0f)
            mVel.z -= mGravity * deltaTime;
        else {
            mPos.z = 0.0f;
            mVel.z = 0.0f;
        }
        getTransform().SetTranslation(glm::vec3(mPos.x, 
                                                mPos.y, 
                                                mHeight + mPos.z));
        getTransform().SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(mTheta)));

        // Camera 
        auto tPos = getTransform().GetLocalTranslation();
        mCamera->setFollowPoint(glm::vec3(tPos.x, tPos.y, 1.0f));
        mCamera->setFollowRot(mTheta-90.0f);
    }

    void ControlledEntity::moveInX(float dx) {
        float deathZone = 0.1;
        mVel.x = (dx > deathZone || dx < -deathZone) ? dx : 0.0f;
    }

    void ControlledEntity::moveInY(float dy) {
        float deathZone = 0.1;
        mVel.y = (dy > deathZone || dy < -deathZone) ? -dy : 0.0f;
    }

    void ControlledEntity::RotInX(float dx) {
        float deathZone = 0.1;
        mRotXVel = (dx > deathZone || dx < -deathZone) ? dx : 0.0f;
    }

    void ControlledEntity::jump() {
        if (mPos.z <= 0.0f)
                mVel.z += 1.0f;
    }
}