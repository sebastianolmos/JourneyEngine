#include "JourneyEngine.hpp"

class LittleDemo : public Journey::Application {
    public:
        LittleDemo() = default;
        ~LittleDemo() = default;

        virtual void UserStartUp(Journey::Scene& scene) override {
            Journey::Entity* floor = new Journey::Entity();
            std::shared_ptr<Journey::Entity> floorPtr = std::shared_ptr<Journey::Entity>(floor);

            floorPtr->getTransform().Set(glm::vec3(-0.0f, 0.0f, 0.0f),
                                    glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(35.0f))),
                                    glm::vec3(100.0f, 70.0f, 000.3f)
                                    );

            Journey::PhongColoredMaterial* floorMat = new Journey::PhongColoredMaterial();
            floorMat->kd = glm::vec3(0.25f, 0.9f, 0.01f);
            floorMat->ke = glm::vec3(0.1f, 0.6f, 0.05f);
            floorMat->ks = glm::vec3(0.1f, 0.1f, 0.1f);

            scene.AddPrimitiveMeshComponent(floorPtr, std::shared_ptr<Journey::Material>(floorMat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, floorPtr);


            Journey::Entity* dog = new Journey::Entity();
            std::shared_ptr<Journey::Entity> dogPtr = std::shared_ptr<Journey::Entity>(dog);

            dogPtr->getTransform().Set(glm::vec3(glm::vec3(0.0f, -0.5f, 1.2f+0.0f)),
                                    glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(35.0f))),
                                    glm::vec3(0.4f, 1.0f, 0.3f)
                                    );


            Journey::SimpleTexturedMaterial* dogMat = new Journey::SimpleTexturedMaterial();

            scene.AddSpriteComponent(dogPtr, std::shared_ptr<Journey::Material>(dogMat), "../../../assets/sprites/carpincho.png");
            scene.AddEntity(nullptr, dogPtr);
            mInnerVar = 0;
        }

        virtual void UserShutDown(Journey::Scene& scene) override {
            mInnerVar = 0;
        }

        virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
            mInnerVar += deltaTime;
        }


    
    private:
        float mInnerVar;
};

int main()
{   
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}