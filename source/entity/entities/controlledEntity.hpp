#include "../entity.hpp"

namespace Journey {
    class FollowCamera;

    class ControlledEntity : public Entity {
     
    public:
        ControlledEntity(std::shared_ptr<FollowCamera> mainCamera);
        ~ControlledEntity() = default;
        virtual void UserStartUp(Scene& scene) override;
        virtual void UserUpdate(Scene& scene, float deltaTime) override;
        void moveInX(float dx);
        void moveInY(float dy);
        void RotInX(float dx);
        void jump();

    protected:
        std::shared_ptr<FollowCamera> mCamera;
        float mTheta = 0.0f;
        glm::vec3 mPos = glm::vec3(0.0f);
        glm::vec3 mVel = glm::vec3(0.0f);
        glm::vec3 mUp = glm::vec3(0.0f, 0.0f, 1.0f);
        float mRotXVel = 0;
        float mHeight = 1.0f;
        float mGravity = 1.0f;
    };

}