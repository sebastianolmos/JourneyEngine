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

class RandRotEntity : public Journey::Entity{
public:
    RandRotEntity() = default;
    ~RandRotEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        mRollVelocity = (rand() / static_cast<float>(RAND_MAX)) * 2.0f;
        mPitchVelocity = (rand() / static_cast<float>(RAND_MAX)) * 2.0f;
        mYawVelocity = (rand() / static_cast<float>(RAND_MAX)) * 2.0f;
        mRoll = (rand() / static_cast<float>(RAND_MAX)) * 7.0f;
        mPitch = (rand() / static_cast<float>(RAND_MAX)) * 7.0f;
        mYaw = (rand() / static_cast<float>(RAND_MAX)) * 7.0f;
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mRoll += mRollVelocity * deltaTime;
        mPitch += mPitchVelocity * deltaTime;
        mYaw += mYawVelocity * deltaTime;
        getTransform().SetRotation(glm::vec3(mRoll, mPitch, mYaw));
    }

    private:
        float mRollVelocity;
        float mPitchVelocity;
        float mYawVelocity;
        float mRoll;
        float mPitch;
        float mYaw;
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
            floorMat->kd = glm::vec3(0.3f, 0.3f, 0.3f);
            floorMat->ke = glm::vec3(0.2f, 0.2f, 0.2f);
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

            // Plane Colored
            std::shared_ptr<RandRotEntity> plane1 = std::make_shared<RandRotEntity>();
            plane1->getTransform().Set(glm::vec3(-18.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(2.0f, 2.0f, 1.0f)
                                    );
            Journey::SimpleColoredMaterial* plane1Mat = new Journey::SimpleColoredMaterial;
            plane1Mat->color = glm::vec3(1.0f, 0.0f, 0.0f);
            scene.AddPrimitiveMeshComponent(plane1, std::shared_ptr<Journey::Material>(plane1Mat), Journey::EPrimitiveMesh::Plane);
            scene.AddEntity(nullptr, plane1);

            // Plane Flat 
            std::shared_ptr<RandRotEntity> plane2 = std::make_shared<RandRotEntity>();
            plane2->getTransform().Set(glm::vec3(-15.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 1.0f)
                                    );
            Journey::FlatColoredMaterial* plane2Mat = new Journey::FlatColoredMaterial;
            plane2Mat->color = glm::vec3(0.0f, 1.0f, 0.5f);
            plane2Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            plane2Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            plane2Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(plane2, std::shared_ptr<Journey::Material>(plane2Mat), Journey::EPrimitiveMesh::Plane);
            scene.AddEntity(nullptr, plane2);

            // Plane Phong
            std::shared_ptr<RandRotEntity> plane3 = std::make_shared<RandRotEntity>();
            plane3->getTransform().Set(glm::vec3(-12.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 1.0f)
                                    );
            Journey::PhongColoredMaterial* plane3Mat = new Journey::PhongColoredMaterial;
            plane3Mat->color = glm::vec3(1.0f, 0.5f, 0.0f);
            plane3Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            plane3Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            plane3Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(plane3, std::shared_ptr<Journey::Material>(plane3Mat), Journey::EPrimitiveMesh::Plane);
            scene.AddEntity(nullptr, plane3);


            // Cube Colored
            std::shared_ptr<RandRotEntity> cube1 = std::make_shared<RandRotEntity>();
            cube1->getTransform().Set(glm::vec3(-9.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::SimpleColoredMaterial* cube1Mat = new Journey::SimpleColoredMaterial;
            cube1Mat->color = glm::vec3(0.0f, 0.5f, 1.0f);
            scene.AddPrimitiveMeshComponent(cube1, std::shared_ptr<Journey::Material>(cube1Mat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, cube1);

            // Cube Flat 
            std::shared_ptr<RandRotEntity> cube2 = std::make_shared<RandRotEntity>();
            cube2->getTransform().Set(glm::vec3(-6.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::FlatColoredMaterial* cube2Mat = new Journey::FlatColoredMaterial;
            cube2Mat->color = glm::vec3(1.0f, 1.0f, 0.0f);
            cube2Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            cube2Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            cube2Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(cube2, std::shared_ptr<Journey::Material>(cube2Mat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, cube2);

            // Cube Phong
            std::shared_ptr<RandRotEntity> cube3 = std::make_shared<RandRotEntity>();
            cube3->getTransform().Set(glm::vec3(-3.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::PhongColoredMaterial* cube3Mat = new Journey::PhongColoredMaterial;
            cube3Mat->color = glm::vec3(0.0f, 0.0f, 1.0f);
            cube3Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            cube3Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            cube3Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(cube3, std::shared_ptr<Journey::Material>(cube3Mat), Journey::EPrimitiveMesh::Cube);
            scene.AddEntity(nullptr, cube3);


            // Cylinder Colored
            std::shared_ptr<RandRotEntity> cylinder1 = std::make_shared<RandRotEntity>();
            cylinder1->getTransform().Set(glm::vec3(3.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::SimpleColoredMaterial* cylinder1Mat = new Journey::SimpleColoredMaterial;
            cylinder1Mat->color = glm::vec3(0.5f, 1.0f, 0.0f);
            scene.AddPrimitiveMeshComponent(cylinder1, std::shared_ptr<Journey::Material>(cylinder1Mat), Journey::EPrimitiveMesh::Cylinder);
            scene.AddEntity(nullptr, cylinder1);

            // Cylinder Flat 
            std::shared_ptr<RandRotEntity> cylinder2 = std::make_shared<RandRotEntity>();
            cylinder2->getTransform().Set(glm::vec3(6.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::FlatColoredMaterial* cylinder2Mat = new Journey::FlatColoredMaterial;
            cylinder2Mat->color = glm::vec3(0.5f, 0.0f, 1.0f);
            cylinder2Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            cylinder2Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            cylinder2Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(cylinder2, std::shared_ptr<Journey::Material>(cylinder2Mat), Journey::EPrimitiveMesh::Cylinder);
            scene.AddEntity(nullptr, cylinder2);

            // Cylinder Phong
            std::shared_ptr<RandRotEntity> cylinder3 = std::make_shared<RandRotEntity>();
            cylinder3->getTransform().Set(glm::vec3(9.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::PhongColoredMaterial* cylinder3Mat = new Journey::PhongColoredMaterial;
            cylinder3Mat->color = glm::vec3(0.0f, 1.0f, 0.0f);
            cylinder3Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            cylinder3Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            cylinder3Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(cylinder3, std::shared_ptr<Journey::Material>(cylinder3Mat), Journey::EPrimitiveMesh::Cylinder);
            scene.AddEntity(nullptr, cylinder3);


            // Sphere Colored
            std::shared_ptr<RandRotEntity> sphere1 = std::make_shared<RandRotEntity>();
            sphere1->getTransform().Set(glm::vec3(12.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::SimpleColoredMaterial* sphere1Mat = new Journey::SimpleColoredMaterial;
            sphere1Mat->color = glm::vec3(1.0f, 0.0f, 1.0f);
            scene.AddPrimitiveMeshComponent(sphere1, std::shared_ptr<Journey::Material>(sphere1Mat), Journey::EPrimitiveMesh::Sphere);
            scene.AddEntity(nullptr, sphere1);

            // sphere Flat 
            std::shared_ptr<RandRotEntity> sphere2 = std::make_shared<RandRotEntity>();
            sphere2->getTransform().Set(glm::vec3(15.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::FlatColoredMaterial* sphere2Mat = new Journey::FlatColoredMaterial;
            sphere2Mat->color = glm::vec3(0.0f, 0.5f, 0.5f);
            sphere2Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            sphere2Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            sphere2Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(sphere2, std::shared_ptr<Journey::Material>(sphere2Mat), Journey::EPrimitiveMesh::Sphere);
            scene.AddEntity(nullptr, sphere2);

            // sphere Phong
            std::shared_ptr<RandRotEntity> sphere3 = std::make_shared<RandRotEntity>();
            sphere3->getTransform().Set(glm::vec3(18.0f, 20.0f, 2.2f),
                                    glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                    glm::vec3(2.0f, 2.0f, 2.0f)
                                    );
            Journey::PhongColoredMaterial* sphere3Mat = new Journey::PhongColoredMaterial;
            sphere3Mat->color = glm::vec3(1.0f, 0.0f, 0.5f);
            sphere3Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
            sphere3Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
            sphere3Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
            scene.AddPrimitiveMeshComponent(sphere3, std::shared_ptr<Journey::Material>(sphere3Mat), Journey::EPrimitiveMesh::Sphere);
            scene.AddEntity(nullptr, sphere3);

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
    srand(time(NULL));
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}