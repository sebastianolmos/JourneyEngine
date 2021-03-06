#include "JourneyEngine.hpp"

void RegisterInputs(Journey::InputController& input) {
    input.RegisterKeyAction("Test", JOURNEY_KEY_SPACE);
    input.RegisterKeyAction("Run", JOURNEY_KEY_LEFT_SHIFT);
    input.RegisterKeyAction("mas", JOURNEY_KEY_K);
    input.RegisterKeyAction("menos", JOURNEY_KEY_J);
    input.RegisterGamepadAxis("MoveX", JOURNEY_GAMEPAD_AXIS_LEFT_X);
    input.RegisterGamepadAxis("MoveY", JOURNEY_GAMEPAD_AXIS_LEFT_Y);
    input.RegisterGamepadAxis("RotX", JOURNEY_GAMEPAD_AXIS_RIGHT_X);
    input.RegisterGamepadAction("Jump", JOURNEY_XBOX_BUTTON_A);
    input.RegisterGamepadAction("Throw", JOURNEY_XBOX_BUTTON_B);
}

void RegisterMeshes(Journey::MeshManager& manager) {
    manager.LoadPrimitiveMesh("projectile", Journey::EPrimitiveMesh::Sphere, true);
    manager.LoadPrimitiveMesh("floorMesh", Journey::EPrimitiveMesh::Cube, true);
    manager.LoadModelMesh("backpackMesh", "../../../assets/bob/bob_the_robot.fbx");
    manager.LoadPrimitiveMesh("plane1", Journey::EPrimitiveMesh::Plane);
    manager.LoadPrimitiveMesh("plane2", Journey::EPrimitiveMesh::Plane, true);
    manager.LoadPrimitiveMesh("cube1", Journey::EPrimitiveMesh::Cube);
    manager.LoadPrimitiveMesh("cube2", Journey::EPrimitiveMesh::Cube, true);
    manager.LoadPrimitiveMesh("cylinder1", Journey::EPrimitiveMesh::Cylinder);
    manager.LoadPrimitiveMesh("cylinder2", Journey::EPrimitiveMesh::Cylinder, true);
    manager.LoadPrimitiveMesh("sphere1", Journey::EPrimitiveMesh::Sphere);
    manager.LoadPrimitiveMesh("sphere2", Journey::EPrimitiveMesh::Sphere, true);
}

void RegisterTextures(Journey::TextureManager& manager) {
    manager.LoadModelTexture("carpincho", "../../../assets/sprites/carpincho.png");
}

class ProjectileEntity : public Journey::Entity {
public:
    ProjectileEntity(glm::vec3 spawnPos, float timeToDelete, float speed, glm::vec3 direction, float size) {
        mTimer = 0.0f;
        mDeleteTime = timeToDelete;
        mSpeed = speed;
        mDirection = direction;
        mPos = spawnPos;
        mSize = size;
    }
    ~ProjectileEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        getTransform().SetTranslation(mPos);
        getTransform().SetScale(glm::vec3(mSize));
        Journey::PhongColoredMaterial* mat = new Journey::PhongColoredMaterial();
        mat->color = glm::vec3(1.0f, 0.5f, 0.0f);
        mat->kd = glm::vec3(0.5f, 0.5f, 0.5f);
        mat->ke = glm::vec3(0.3f, 0.3f, 0.3f);
        mat->ks = glm::vec3(0.3f, 0.3f, 0.3f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "projectile");
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mPos += mDirection * mSpeed * deltaTime;
        getTransform().SetTranslation(mPos);

        mTimer += deltaTime;
        if (mTimer > mDeleteTime)
            mManager->DeleteEntity(shared_from_this());
    }
private:
    float mTimer;
    float mDeleteTime;
    float mSpeed, mSize;
    glm::vec3 mDirection;
    glm::vec3 mPos;
};

class Carpincho : public Journey::ControlledEntity{
public:
    Carpincho(std::shared_ptr<Journey::FollowCamera> mainCamera)
    : Journey::ControlledEntity(mainCamera)
    {}
    ~Carpincho() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        this->getTransform().Set(glm::vec3(-2.0f, -2.5f, 1.0f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.65f, 1.0f, 0.42f)
                                );
        Journey::SimpleTexturedMaterial* mat = new Journey::SimpleTexturedMaterial();
        Journey::TextureManager::getInstance().AddSpriteComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "carpincho", true);
        // Input
        scene.GetInputController().BindAxisMap("MoveX", [&](float dx) {this->moveInX(dx);});
        scene.GetInputController().BindAxisMap("MoveY", [&](float dy) {this->moveInY(dy);});
        scene.GetInputController().BindAxisMap("RotX", [&](float dx) {this->RotInX(dx);});
        scene.GetInputController().BindActionOnReleased("Jump", [&]() {this->jump();});
        scene.GetInputController().BindActionOnReleased("Throw", [&]() {this->throwProjectile();});
        mCamera->setCenterHeight(0.9f);
        mCamera->setDistanceRadius(4.0f);
        mCamera->setThetaAngle(85.0f);
        mCamera->setFov(55.0f);
    }
    void throwProjectile() {
        std::shared_ptr<ProjectileEntity> projectile = std::make_shared<ProjectileEntity>(
            mPos + mUp*mHeight, 1.0f, 8.0f, glm::vec3(getFixedVel().x, getFixedVel().y, 0.0f), 0.2f
        );
        mManager->AddEntity(nullptr, projectile);
    }
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

class TestDemo : public Journey::Application {
public:
    TestDemo() {
        mName = "Test-Demo";
        mWidth = 1200;
        mHeight = 800;
    }
    ~TestDemo() = default;

    virtual void UserStartUp(Journey::Scene& scene) override {
        scene.DebugModeEnabled();

        // Registers
        RegisterInputs(scene.GetInputController());
        RegisterMeshes(Journey::MeshManager::getInstance());
        RegisterTextures(Journey::TextureManager::getInstance());
        
        scene.GetInputController().BindActionOnPressed("Test", [&]() {this->testo();});
        scene.GetInputController().BindActionOnPressed("mas", [&]() {this->mas();});
        scene.GetInputController().BindActionOnPressed("menos", [&]() {this->menos();});
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

        auto& meshManager = Journey::MeshManager::getInstance();
        meshManager.AddMeshComponent(floor, std::shared_ptr<Journey::Material>(floorMat), "floorMesh");
        scene.AddEntity(nullptr, floor);

        // Entity creation - method 1
        dog = std::make_shared<Journey::Entity>();
        dog->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(0.4f, 1.0f, 0.3f)
                                );
        Journey::SimpleTexturedMaterial* dogMat = new Journey::SimpleTexturedMaterial();

        auto& textureManager = Journey::TextureManager::getInstance();
        textureManager.AddSpriteComponent(dog, std::shared_ptr<Journey::Material>(dogMat), "carpincho", true);
        scene.AddEntity(nullptr, dog);

        // Set Camera
        mainCamera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
        scene.GetCameraHandler().setCurrentCamera(mainCamera);

        // Entity creation - method 2
        carpin = std::make_shared<Carpincho>(mainCamera);
        scene.AddEntity(nullptr, carpin);

        // Add point Light 1
        std::shared_ptr<Journey::Entity> light1 = std::make_shared<Journey::Entity>();
        light1->getTransform().SetTranslation(glm::vec3(1.5f, 1.0f, 5.5f));
        auto lComp1 = scene.AddPointLightComponent(light1);
        lComp1->setAmbient(glm::vec3(0.2f));
        lComp1->setDiffuse(glm::vec3(1.0f));
        lComp1->setSpecular(glm::vec3(1.0f));
        lComp1->turnOff();
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 2; j++) {
                std::shared_ptr<Journey::Entity> light = std::make_shared<Journey::Entity>();
                light->getTransform().SetTranslation(glm::vec3(-18.0f + i*5.0f, 18.0f + j*5.0f, 2.2f));
                auto lComp = scene.AddPointLightComponent(light);
                lComp->setAmbient(glm::vec3(0.0f));
                lComp->setDiffuse(glm::vec3(0.0f+ 1.0f/7.0f*i, 1.0f- 1.0f/7.0f*i, 0.0 + 1.0f*j));
                lComp->setSpecular(glm::vec3(1.0f));
                lComp->turnOn();
                scene.AddEntity(nullptr, light);
            }
        }

        // Add spot Light 1
        std::shared_ptr<Journey::Entity> spot1 = std::make_shared<Journey::Entity>();
        spot1->getTransform().SetTranslation(glm::vec3(0.0f));
        auto sComp1 = scene.AddSpotLightComponent(spot1);
        sComp1->setDirection(glm::vec3(1.0f, 0.0f, -0.7f));
        sComp1->setAmbient(glm::vec3(0.0f));
        sComp1->setDiffuse(glm::vec3(1.0f, 0.7f, 0.0f));
        sComp1->setSpecular(glm::vec3(1.0f));
        sComp1->setCutOff(glm::cos(glm::radians(26.0f)));
        sComp1->setOuterCutOff(glm::cos(glm::radians(40.0f)));
        sComp1->setConstantAttenuation(0.5f);
        sComp1->setLinearAttenuation(0.02f);
        sComp1->setQuadraticAttenuation(0.002f);
        sComp1->turnOn();
        scene.AddEntity(carpin, spot1);     
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 2; j++) {
                std::shared_ptr<Journey::Entity> spotl = std::make_shared<Journey::Entity>();
                spotl->getTransform().SetTranslation(glm::vec3(-18.0f + i*4.0f, -23.0f + j*4.0f, 2.5f));
                auto sComp = scene.AddSpotLightComponent(spotl);
                sComp->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
                sComp->setAmbient(glm::vec3(0.0f));
                sComp->setDiffuse(glm::vec3(1.0f));
                sComp->setSpecular(glm::vec3(1.0f));
                sComp->setCutOff(glm::cos(glm::radians(30.0f)));
                sComp->setOuterCutOff(glm::cos(glm::radians(35.0f)));
                sComp->setConstantAttenuation(0.5f);
                sComp->setLinearAttenuation(0.02f);
                sComp->setQuadraticAttenuation(0.002f);
                sComp->turnOn();
                scene.AddEntity(nullptr, spotl);  
            }
        }   

        // Add skybox
        std::vector<std::string> faces
        {
            "../../../assets/skybox/right.png",
            "../../../assets/skybox/left.png",
            "../../../assets/skybox/top.png",
            "../../../assets/skybox/bottom.png",
            "../../../assets/skybox/front.png",
            "../../../assets/skybox/back.png"
        };
        scene.AddSkybox(faces);

        // Add a skeletal mesh with animations
        std::shared_ptr<Journey::Entity> bob = std::make_shared<Journey::Entity>();
        bob->getTransform().Set(glm::vec3(-2.0f, 2.0f, 1.2f),
                                glm::vec3(glm::radians(90.0f), 0.0f, 0.0f),
                                glm::vec3(0.01)
                                );
        skComp = scene.GetSkeletalManager().AddSkeletalMeshComponent(bob, "../../../assets/trex/T_Rex.fbx", glm::vec3(1.0f));
        scene.AddEntity(nullptr, bob);
        if (skComp->model){
            skComp->model->LoadAnimation(0, "0", false);
            skComp->model->LoadAnimation(1, "1", false);
            skComp->model->LoadAnimation(2, "2", false);
            skComp->model->LoadAnimation(3, "3", false);
            skComp->model->LoadAnimation(4, "4", false);
            skComp->model->LoadAnimation(5, "5", false);
            skComp->model->LoadAnimation(6, "6", false);
            skComp->model->LoadAnimation(7, "7", false);
            skComp->model->LoadAnimation(8, "8", false);
            skComp->model->LoadAnimation(9, "9", false);
            skComp->model->LoadAnimation(10, "10", false);
            skComp->model->LoadAnimation(11, "11", false);
            skComp->model->LoadAnimation(12, "12", false);
            skComp->model->LoadAnimation(13, "13", false);
            skComp->model->LoadAnimation(14, "14", false);
            skComp->model->LoadAnimation(15, "15", false);
            skComp->model->PlayAnimation("0");
        }
        

        //backpack test
        std::shared_ptr<Journey::Entity> backpack = std::make_shared<Journey::Entity>();
        backpack->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(glm::radians(90.0f), 0.0f, 0.0f),
                                glm::vec3(0.03f)
                                );
        Journey::PhongTexturedMaterial* backpackMat = new Journey::PhongTexturedMaterial();
        backpackMat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
        backpackMat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        backpackMat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
        meshManager.AddMeshComponent(backpack, std::shared_ptr<Journey::Material>(backpackMat), "backpackMesh");
        scene.AddEntity(nullptr, backpack);

        // Plane Colored
        std::shared_ptr<RandRotEntity> plane1 = std::make_shared<RandRotEntity>();
        plane1->getTransform().Set(glm::vec3(-18.0f, 20.0f, 2.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(2.0f, 2.0f, 1.0f)
                                );
        Journey::SimpleColoredMaterial* plane1Mat = new Journey::SimpleColoredMaterial;
        plane1Mat->color = glm::vec3(1.0f, 0.0f, 0.0f);
        meshManager.AddMeshComponent(plane1, std::shared_ptr<Journey::Material>(plane1Mat), "plane1");
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
        meshManager.AddMeshComponent(plane2, std::shared_ptr<Journey::Material>(plane2Mat), "plane2");
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
        meshManager.AddMeshComponent(plane3, std::shared_ptr<Journey::Material>(plane3Mat), "plane2");
        scene.AddEntity(nullptr, plane3);


        // Cube Colored
        std::shared_ptr<RandRotEntity> cube1 = std::make_shared<RandRotEntity>();
        cube1->getTransform().Set(glm::vec3(-9.0f, 20.0f, 2.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(2.0f, 2.0f, 2.0f)
                                );
        Journey::SimpleColoredMaterial* cube1Mat = new Journey::SimpleColoredMaterial;
        cube1Mat->color = glm::vec3(0.0f, 0.5f, 1.0f);
        meshManager.AddMeshComponent(cube1, std::shared_ptr<Journey::Material>(cube1Mat), "cube1");
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
        meshManager.AddMeshComponent(cube2, std::shared_ptr<Journey::Material>(cube2Mat), "cube2");
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
        meshManager.AddMeshComponent(cube3, std::shared_ptr<Journey::Material>(cube3Mat), "cube2");
        scene.AddEntity(nullptr, cube3);


        // Cylinder Colored
        std::shared_ptr<RandRotEntity> cylinder1 = std::make_shared<RandRotEntity>();
        cylinder1->getTransform().Set(glm::vec3(3.0f, 20.0f, 2.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(2.0f, 2.0f, 2.0f)
                                );
        Journey::SimpleColoredMaterial* cylinder1Mat = new Journey::SimpleColoredMaterial;
        cylinder1Mat->color = glm::vec3(4.0f, 2.5f, 1.0f);
        meshManager.AddMeshComponent(cylinder1, std::shared_ptr<Journey::Material>(cylinder1Mat), "cylinder1");
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
        meshManager.AddMeshComponent(cylinder2, std::shared_ptr<Journey::Material>(cylinder2Mat), "cylinder2");
        scene.AddEntity(nullptr, cylinder2);

        // Cylinder Phong
        std::shared_ptr<RandRotEntity> cylinder3 = std::make_shared<RandRotEntity>();
        cylinder3->getTransform().Set(glm::vec3(9.0f, 20.0f, 2.2f),
                                glm::vec3(0.0f, glm::radians(-55.0f), glm::radians(-35.0f)),
                                glm::vec3(2.0f, 2.0f, 2.0f)
                                );
        Journey::PhongColoredMaterial* cylinder3Mat = new Journey::PhongColoredMaterial;
        cylinder3Mat->color = glm::vec3(1.0f, 1.0f, 1.0f);
        cylinder3Mat->kd = glm::vec3(0.6f, 0.6f, 0.6f)*3.0f;
        cylinder3Mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        cylinder3Mat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
        meshManager.AddMeshComponent(cylinder3, std::shared_ptr<Journey::Material>(cylinder3Mat), "cylinder2");
        scene.AddEntity(nullptr, cylinder3);


        // Sphere Colored
        std::shared_ptr<RandRotEntity> sphere1 = std::make_shared<RandRotEntity>();
        sphere1->getTransform().Set(glm::vec3(12.0f, 20.0f, 2.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(2.0f, 2.0f, 2.0f)
                                );
        Journey::SimpleColoredMaterial* sphere1Mat = new Journey::SimpleColoredMaterial;
        sphere1Mat->color = glm::vec3(1.0f, 0.0f, 1.0f);
        meshManager.AddMeshComponent(sphere1, std::shared_ptr<Journey::Material>(sphere1Mat), "sphere1");
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
        meshManager.AddMeshComponent(sphere2, std::shared_ptr<Journey::Material>(sphere2Mat), "sphere2");
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
        meshManager.AddMeshComponent(sphere3, std::shared_ptr<Journey::Material>(sphere3Mat), "sphere2");
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

    void mas(){
        anim = (anim + 1) % 16;
        skComp->model->PlayAnimation(std::to_string(anim));
        std::cout << "Anim playing: " << anim << std::endl;
    }

    void menos(){
        anim = (anim - 1) % 16;
        skComp->model->PlayAnimation(std::to_string(anim));
        std::cout << "Anim playing: " << anim << std::endl;
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
    std::shared_ptr<Journey::FollowCamera> mainCamera;
    std::shared_ptr<Journey::SkeletalMeshComponent> skComp;
    int anim = 0;
};

int main()
{   
    srand(time(NULL));
    TestDemo testDemo;
    Journey::Engine engine(testDemo);
    engine.StartMainLoop();

    return 0;
}