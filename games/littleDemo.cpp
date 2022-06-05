#include "JourneyEngine.hpp"

void RegisterInputs(Journey::InputController& input) {
    input.RegisterKeyAction("Jump", JOURNEY_KEY_SPACE);
    input.RegisterGamepadAction("Test", JOURNEY_XBOX_BUTTON_A);
    input.RegisterKeyAction("Run", JOURNEY_KEY_LEFT_SHIFT);
    input.RegisterGamepadAxis("MoveX", JOURNEY_GAMEPAD_AXIS_LEFT_X);
    input.RegisterGamepadAxis("MoveY", JOURNEY_GAMEPAD_AXIS_LEFT_Y);
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
            // Input
            scene.GetInputController().BindActionOnReleased("Test", [&]() {this->insideFuncTest();});
            scene.GetInputController().BindAxisMap("MoveX", [&](float dx) {this->moveInX(dx);});
            scene.GetInputController().BindAxisMap("MoveY", [&](float dy) {this->moveInY(dy);});
        }
        virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
            mTimer += deltaTime;
            mPosX += deltaTime * mVelX;
            mPosY += deltaTime * mVelY;
            getTransform().SetTranslation(glm::vec3(0.0f + mPosX, 
                                                    0.5f + mPosY, 
                                                    1.0f + 0.7*glm::abs(glm::sin(mTimer +0.4))));
        };
        void insideFuncTest(){
            std::cout << "Working god!" << std::endl;
        }
        void moveInX(float dx) {
            float deathZone = 0.1;
            mVelX = (dx > deathZone || dx < -deathZone) ? dx : 0.0f;
        }
        void moveInY(float dy) {
            float deathZone = 0.1;
            mVelY = (dy > deathZone || dy < -deathZone) ? -dy : 0.0f;
        }
    private:
        float mTimer;
        float mVelX = 0;
        float mVelY = 0;
        float mPosX = 0.0f;
        float mPosY = 0.0f;
};

class LittleDemo : public Journey::Application {
    public:
        LittleDemo() {
            mName = "Little Demo :3";
            mWidth = 1200;
            mHeight = 800;
        }
        ~LittleDemo() = default;

        virtual void UserStartUp(Journey::Scene& scene) override {
            scene.DebugModeEnabled();

            // Input 
            RegisterInputs(scene.GetInputController());
            scene.GetInputController().BindActionOnPressed("Jump", [&]() {this->testo();});
            scene.GetInputController().BindActionToggle("Run", [&](bool v) {this->changeRunState(v);});

            // Floor entity
            floor = std::make_shared<Journey::Entity>();
            floor->getTransform().Set(glm::vec3(-0.0f, 0.0f, -0.2f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(100.0f, 70.0f, 0.3f)
                                    );

            Journey::PhongColoredMaterial* floorMat = new Journey::PhongColoredMaterial();
            floorMat->color = glm::vec3(0.25f, 0.9f, 0.01f);
            floorMat->kd = glm::vec3(0.1f, 0.1f, 0.1f);
            floorMat->ke = glm::vec3(0.2f, 0.3f, 0.1f);
            floorMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);

            scene.AddPrimitiveMeshComponent(floor, std::shared_ptr<Journey::Material>(floorMat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, floor);

            // Entity creation - method 1
            dog = std::make_shared<Journey::Entity>();
            dog->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(0.4f, 1.0f, 0.3f)
                                    );
            Journey::SimpleTexturedMaterial* dogMat = new Journey::SimpleTexturedMaterial();
            scene.AddSpriteComponent(dog, std::shared_ptr<Journey::Material>(dogMat), "../../../assets/sprites/carpincho.png");
            scene.AddEntity(nullptr, dog);

            // Entity creation - method 2
            carpin = std::make_shared<Carpincho>();
            scene.AddEntity(nullptr, carpin);

            // Set Camera
            std::shared_ptr<Journey::FollowCamera> mainCamera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
            scene.GetCameraHandler().setCurrentCamera(mainCamera);

            // Plane1 Test 
            std::shared_ptr<Journey::Entity> plane1 = std::make_shared<Journey::Entity>();
            plane1->getTransform().Set(glm::vec3(10.0f, 10.0f, 2.2f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(2.0f, 2.0f, 1.0f)
                                    );

            Journey::SimpleColoredMaterial* plane1Mat = new Journey::SimpleColoredMaterial;
            plane1Mat->color = glm::vec3(0.25f, 0.1f, 0.9f);
            scene.AddPrimitiveMeshComponent(plane1, std::shared_ptr<Journey::Material>(plane1Mat), Journey::EPrimitiveMesh::Plane);
            scene.AddEntity(nullptr, plane1);

            // Plane2 Test 
            std::shared_ptr<Journey::Entity> plane2 = std::make_shared<Journey::Entity>();
            plane2->getTransform().Set(glm::vec3(10.0f, -10.0f, 0.3f),
                                    glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                    glm::vec3(2.0f, 2.0f, 1.0f)
                                    );

            Journey::PhongColoredMaterial* plane2Mat = new Journey::PhongColoredMaterial;
            plane2Mat->color = glm::vec3(0.95f, 0.6f, 0.2f);
            plane2Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            plane2Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            plane2Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);

            scene.AddPrimitiveMeshComponent(plane2, std::shared_ptr<Journey::Material>(plane2Mat), Journey::EPrimitiveMesh::Plane);
            scene.AddEntity(nullptr, plane2);

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