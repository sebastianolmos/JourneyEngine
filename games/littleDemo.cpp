#include "JourneyEngine.hpp"

class LittleDemo : public Journey::Application {
    public:
        LittleDemo() = default;
        ~LittleDemo() = default;

        virtual void UserStartUp(Journey::Scene& scene) override {
            floor = std::make_shared<Journey::Entity>();

            floor->getTransform().Set(glm::vec3(-0.0f, 0.0f, 0.0f),
                                    glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(35.0f))),
                                    glm::vec3(100.0f, 70.0f, 000.3f)
                                    );

            Journey::PhongColoredMaterial* floorMat = new Journey::PhongColoredMaterial();
            floorMat->kd = glm::vec3(0.25f, 0.9f, 0.01f);
            floorMat->ke = glm::vec3(0.1f, 0.6f, 0.05f);
            floorMat->ks = glm::vec3(0.1f, 0.1f, 0.1f);

            scene.AddPrimitiveMeshComponent(floor, std::shared_ptr<Journey::Material>(floorMat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, floor);


            dog = std::make_shared<Journey::Entity>();

            dog->getTransform().Set(glm::vec3(glm::vec3(0.0f, -0.5f, 1.2f+0.0f)),
                                    glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(35.0f))),
                                    glm::vec3(0.4f, 1.0f, 0.3f)
                                    );


            Journey::SimpleTexturedMaterial* dogMat = new Journey::SimpleTexturedMaterial();

            scene.AddSpriteComponent(dog, std::shared_ptr<Journey::Material>(dogMat), "../../../assets/sprites/carpincho.png");
            scene.AddEntity(nullptr, dog);
            mInnerVar = 0;
        }

        virtual void UserShutDown(Journey::Scene& scene) override {
            mInnerVar = 0;
        }

        virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
            mInnerVar += deltaTime;
            glm::vec3 tmpTr = dog->getTransform().GetLocalTranslation() + glm::vec3(mInnerVar, 0, 0);
            dog->getTransform().SetTranslation(glm::vec3(mInnerVar,-0.5f, 1.2f));
            dog->getTransform().SetRotation(glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(mInnerVar))));
        }

    private:
        float mInnerVar;
        std::shared_ptr<Journey::Entity> floor;
        std::shared_ptr<Journey::Entity> dog;
};

int main()
{   
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}