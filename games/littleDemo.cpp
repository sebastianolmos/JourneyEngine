#include "JourneyEngine.hpp"

void RegisterInputs(Journey::InputController& input) {
    input.RegisterGamepadAxis("MoveX", JOURNEY_GAMEPAD_AXIS_LEFT_X);
    input.RegisterGamepadAxis("MoveY", JOURNEY_GAMEPAD_AXIS_LEFT_Y);
    input.RegisterGamepadAxis("RotX", JOURNEY_GAMEPAD_AXIS_RIGHT_X);
    input.RegisterGamepadAction("Jump", JOURNEY_XBOX_BUTTON_A);
    input.RegisterGamepadAction("Throw", JOURNEY_XBOX_BUTTON_B);
}

void RegisterMeshes(Journey::MeshManager& manager) {
    manager.LoadPrimitiveMesh("projectile", Journey::EPrimitiveMesh::Sphere, true);
    manager.LoadPrimitiveMesh("floorMesh", Journey::EPrimitiveMesh::Cube, true);
    manager.LoadModelMesh("shibaMesh", "../../../assets/shiba/shiba.obj");
    manager.LoadModelMesh("cloudMesh", "../../../assets/cloud/cloudy.obj");
    manager.LoadModelMesh("terrainMesh", "../../../assets/farm/farm.fbx");
}

void RegisterTextures(Journey::TextureManager& manager) {
    manager.LoadModelTexture("carpincho", "../../../assets/sprites/carpincho.png");
}

class ProjectileEntity : public Journey::Entity {
public:
    ProjectileEntity(glm::vec3 spawnPos, float timeToDelete, float speed, glm::vec3 direction, float size, float rotZ) {
        mTimer = 0.0f;
        mDeleteTime = timeToDelete;
        mSpeed = speed;
        mDirection = direction;
        mPos = spawnPos;
        mSize = size;
        mRotation = rotZ;
    }
    ~ProjectileEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        getTransform().SetTranslation(mPos);
        getTransform().SetScale(glm::vec3(mSize));
        getTransform().SetRotation(glm::vec3(glm::radians(90.0f), 0.0f, mRotation));
        Journey::PhongColoredMaterial* mat = new Journey::PhongColoredMaterial();
        mat->color = glm::vec3(0.95f, 0.95f, 0.95f);
        mat->kd = glm::vec3(0.5f, 0.5f, 0.56f);
        mat->ke = glm::vec3(0.3f, 0.3f, 0.35f);
        mat->ks = glm::vec3(0.3f, 0.3f, 0.35f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "cloudMesh");
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
    float mSpeed, mSize, mRotation;
    glm::vec3 mDirection;
    glm::vec3 mPos;
};

class Shiba : public Journey::ControlledEntity{
public:
    Shiba(std::shared_ptr<Journey::FollowCamera> mainCamera)
    : Journey::ControlledEntity(mainCamera)
    {}
    ~Shiba() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        this->getTransform().Set(glm::vec3(-2.0f, -2.5f, 0.5f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.6f, 0.6f, 0.6f)
                                );
        Journey::PhongTexturedMaterial* mat = new Journey::PhongTexturedMaterial();
        mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
        mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        mat->ks = glm::vec3(0.0f, 0.0f, 0.0f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "shibaMesh");
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
        mHeight = 0.3;
        mRotSpeed = 80.0f;
    }
    void throwProjectile() {
        std::shared_ptr<ProjectileEntity> projectile = std::make_shared<ProjectileEntity>(
            mPos + mUp*mHeight*2.0f, 1.0f, 8.0f, glm::vec3(getFixedVel().x, getFixedVel().y, 0.0f), 0.2f, mAngleZ + glm::radians(90.0f)
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

        // Registers
        RegisterInputs(scene.GetInputController());
        RegisterMeshes(Journey::MeshManager::getInstance());
        RegisterTextures(Journey::TextureManager::getInstance());

        auto& meshManager = Journey::MeshManager::getInstance();
        // Floor Terrain entity
        terrain = std::make_shared<Journey::Entity>();
        terrain->getTransform().Set(glm::vec3(-0.0f, 0.0f, -0.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(5.0f, 5.0f, 5.0f)
                                );
        Journey::PhongTexturedMaterial* terrainMat = new Journey::PhongTexturedMaterial();
        terrainMat->kd = glm::vec3(0.5f, 0.5f, 0.5f);
        terrainMat->ke = glm::vec3(0.4f, 0.4f, 0.4f);
        terrainMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);
        meshManager.AddMeshComponent(terrain, std::shared_ptr<Journey::Material>(terrainMat), "terrainMesh");
        scene.AddEntity(nullptr, terrain);

        // Entity creation - method 1
        carpin = std::make_shared<Journey::Entity>();
        carpin->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(0.4f, 1.0f, 0.3f)
                                );
        Journey::SimpleTexturedMaterial* carpinMat = new Journey::SimpleTexturedMaterial();

        auto& textureManager = Journey::TextureManager::getInstance();
        textureManager.AddSpriteComponent(carpin, std::shared_ptr<Journey::Material>(carpinMat), "carpincho", true);
        scene.AddEntity(nullptr, carpin);

        // Set Camera
        mainCamera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
        scene.GetCameraHandler().setCurrentCamera(mainCamera);

        //Set light 
        scene.GetPointLight().setLightPos(glm::vec3(30.0f, -30.0f, 10.0f));

        // Entity creation - method 2
        shiba = std::make_shared<Shiba>(mainCamera);
        scene.AddEntity(nullptr, shiba);

        mInnerVar = 0;
    }

    virtual void UserShutDown(Journey::Scene& scene) override {
        mInnerVar = 0;
    }

    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mInnerVar += deltaTime;
        carpin->getTransform().SetTranslation(glm::vec3(glm::sin(mInnerVar)*1.2,-0.5f, 1.2f));
        carpin->getTransform().SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(35.0f) + mInnerVar));
    }

private:
    float mInnerVar;
    std::shared_ptr<Journey::Entity> terrain;
    std::shared_ptr<Journey::Entity> carpin;
    std::shared_ptr<Shiba> shiba;
    std::shared_ptr<Journey::FollowCamera> mainCamera;
};

int main()
{   
    srand(time(NULL));
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}