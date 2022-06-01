#include "JourneyEngine.hpp"

void RegisterInputs(Journey::InputController& input) {
    input.RegisterKeyAction("Jump", JOURNEY_KEY_SPACE);
    input.RegisterKeyAction("Test", JOURNEY_KEY_W);
    input.RegisterKeyAction("Run", JOURNEY_KEY_LEFT_SHIFT);
}

class Carpincho : public Journey::Entity{
    public:
        Carpincho() = default;
        ~Carpincho() = default;
        virtual void UserStartUp(Journey::Scene& scene) override {
            mTimer = 0.0f;
            this->getTransform().Set(glm::vec3(-2.0f, -2.5f, 1.0f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(15.0f)),
                                    glm::vec3(0.65f, 1.0f, 0.42f)
                                    );
            Journey::SimpleTexturedMaterial* mat = new Journey::SimpleTexturedMaterial();
            scene.AddSpriteComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "../../../assets/sprites/carpincho.png");
        }
        virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
            mTimer += deltaTime;
            getTransform().SetTranslation(glm::vec3(0.0f, 0.5f, 1.0f + 0.7*glm::abs(glm::sin(mTimer +0.4))));
        };
        void insideFuncTest(){
            std::cout << "Working god!" << std::endl;
        }
    private:
        float mTimer;
};

class LittleDemo : public Journey::Application {
    public:
        LittleDemo() = default;
        ~LittleDemo() = default;

        virtual void UserStartUp(Journey::Scene& scene) override {
            RegisterInputs(scene.GetInputController());
            scene.GetInputController().BindActionOnPressed("Jump", [&]() {this->testo();});

            floor = std::make_shared<Journey::Entity>();
            floor->getTransform().Set(glm::vec3(-0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(100.0f, 70.0f, 0.3f)
                                    );

            Journey::PhongColoredMaterial* floorMat = new Journey::PhongColoredMaterial();
            floorMat->color = glm::vec3(0.25f, 0.9f, 0.01f);
            floorMat->kd = glm::vec3(0.25f, 0.25f, 0.01f);
            floorMat->ke = glm::vec3(0.3f, 0.7f, 0.1f);
            floorMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);

            scene.AddPrimitiveMeshComponent(floor, std::shared_ptr<Journey::Material>(floorMat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, floor);


            dog = std::make_shared<Journey::Entity>();
            dog->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(0.4f, 1.0f, 0.3f)
                                    );

            Journey::SimpleTexturedMaterial* dogMat = new Journey::SimpleTexturedMaterial();

            scene.AddSpriteComponent(dog, std::shared_ptr<Journey::Material>(dogMat), "../../../assets/sprites/carpincho.png");
            scene.AddEntity(nullptr, dog);

            carpin = std::make_shared<Carpincho>();
            scene.AddEntity(nullptr, carpin);
            scene.GetInputController().BindActionOnReleased("Test", [&]() {carpin->insideFuncTest();});
            scene.GetInputController().BindActionToggle("Run", [&](bool v) {this->changeRunState(v);});

            mInnerVar = 0;
        }

        virtual void UserShutDown(Journey::Scene& scene) override {
            mInnerVar = 0;
        }

        virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
            mInnerVar += deltaTime;
            dog->getTransform().SetTranslation(glm::vec3(glm::sin(mInnerVar)*1.2,-0.5f, 1.2f));
            dog->getTransform().SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(35.0f) + mInnerVar));
        }

        void testo(){
            std::cout << "Looking god!" << std::endl;
        }

        void changeRunState(bool state) {
            if (state)
                std::cout << "Start Running!" << std::endl;
            else 
                std::cout << "Stop Running!" << std::endl;
        }

    private:
        float mInnerVar;
        std::shared_ptr<Journey::Entity> floor;
        std::shared_ptr<Journey::Entity> dog;
        std::shared_ptr<Carpincho> carpin;
};

int main()
{   
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}