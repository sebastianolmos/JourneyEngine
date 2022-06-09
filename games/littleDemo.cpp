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
    manager.LoadPrimitiveMesh("sunMesh", Journey::EPrimitiveMesh::Sphere, false);
    manager.LoadPrimitiveMesh("sateliteMesh", Journey::EPrimitiveMesh::Cylinder, true);
    manager.LoadPrimitiveMesh("grassMesh", Journey::EPrimitiveMesh::Plane, true);
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
        mRunSpeed = 6.0f;
        mPos.y = -17.0f;
    }
    void throwProjectile() {
        std::shared_ptr<ProjectileEntity> projectile = std::make_shared<ProjectileEntity>(
            mPos + mUp * mHeight * 2.0f, 
            2.0f, 
            8.0f + glm::length(mVel)*mRunSpeed, 
            glm::vec3(getFixedVel().x, getFixedVel().y, 0.0f), 
            0.2f, mAngleZ + glm::radians(90.0f)
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
    float mRollVelocity, mPitchVelocity, mYawVelocity, mRoll, mPitch, mYaw;
};


class SunEntity : public Journey::Entity {
public:
    SunEntity() = default;
    ~SunEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        Journey::SimpleColoredMaterial* mat = new Journey::SimpleColoredMaterial();
        mat->color = glm::vec3(1.0f, 1.0f, 0.7f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "sunMesh");
        getTransform().SetTranslation(scene.GetPointLight().getLightPos());
        getTransform().SetScale(glm::vec3(3.0f));

        // Add the satelite
        mSatelite = std::make_shared<RandRotEntity>();
        Journey::FlatColoredMaterial* satMat = new Journey::FlatColoredMaterial();
        satMat->color = glm::vec3(0.5f, 0.0f, 1.0f);
        satMat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
        satMat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        satMat->ks = glm::vec3(1.0f, 0.8f, 0.8f);
        Journey::MeshManager::getInstance().AddMeshComponent(mSatelite, std::shared_ptr<Journey::Material>(satMat), "sateliteMesh");
        mSatelite->getTransform().SetTranslation(glm::vec3(1.5, 0.0, 0.0));
        mSatelite->getTransform().SetScale(glm::vec3(0.4f));
        mManager->AddEntity(shared_from_this(), mSatelite);
        mSateliteRot = 0.0f;
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mSateliteRot += deltaTime;
        getTransform().SetRotation(glm::vec3(0.0f, 0.0f, mSateliteRot));
    }

private:
    std::shared_ptr<RandRotEntity> mSatelite;
    float mSateliteRot;
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
        // User can use debug mode pressing Ctrl + D
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

        // Set Camera
        mainCamera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
        scene.GetCameraHandler().setCurrentCamera(mainCamera);
        mainCamera->setFov(45.0f);

        //Set light 
        scene.GetPointLight().setLightPos(glm::vec3(20.0f, -20.0f, 10.0f));

        // Entity creation - method 2
        shiba = std::make_shared<Shiba>(mainCamera);
        scene.AddEntity(nullptr, shiba);

        // Add the sun + satelite
        auto sun = std::make_shared<SunEntity>();
        scene.AddEntity(nullptr, sun);

        // Add the plane thta represnt the grass
        auto grass = std::make_shared<Journey::Entity>();
        grass->getTransform().Set(glm::vec3(0.0f, 0.0f, -0.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(100.0f, 70.0f, 0.3f)
                                );
        Journey::PhongColoredMaterial* grassMat = new Journey::PhongColoredMaterial();
        grassMat->color = glm::vec3(0.25f, 0.9f, 0.01f);
        grassMat->kd = glm::vec3(0.3f, 0.3f, 0.3f);
        grassMat->ke = glm::vec3(0.2f, 0.2f, 0.2f);
        grassMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);
        meshManager.AddMeshComponent(grass, std::shared_ptr<Journey::Material>(grassMat), "grassMesh");
        scene.AddEntity(nullptr, grass);

        auto& textureManager = Journey::TextureManager::getInstance();

        // Carpinchos to test the draw og images with transparency
        // CARPINCHO 1
        Journey::SimpleTexturedMaterial* carpinMat = new Journey::SimpleTexturedMaterial();
        carpin1 = std::make_shared<Journey::Entity>();
        carpin1->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(1.2f, 1.0f, 0.75f)
                                );
        textureManager.AddSpriteComponent(carpin1, std::shared_ptr<Journey::Material>(carpinMat), "carpincho", true);
        scene.AddEntity(nullptr, carpin1);
        // CARPINCHO 2
        carpin2 = std::make_shared<Journey::Entity>();
        carpin2->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(1.44f, 1.0f, 0.9f)
                                );
        textureManager.AddSpriteComponent(carpin2, std::shared_ptr<Journey::Material>(carpinMat), "carpincho", true);
        scene.AddEntity(nullptr, carpin2);
        // CARPINCHO 3
        carpin3 = std::make_shared<Journey::Entity>();
        carpin3->getTransform().Set(glm::vec3(0.0f, -0.5f, 1.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(1.5f, 1.0f, 1.0f)
                                );
        textureManager.AddSpriteComponent(carpin3, std::shared_ptr<Journey::Material>(carpinMat), "carpincho", true);
        scene.AddEntity(nullptr, carpin3);
        // CARPINCHO 4
        carpin4 = std::make_shared<Journey::Entity>();
        carpin4->getTransform().Set(glm::vec3(0.0f, -0.5f, 0.5f),
                                glm::vec3(0.0f, 0.0f, glm::radians(35.0f)),
                                glm::vec3(1.4f, 1.0f, 1.0f)
                                );
        textureManager.AddSpriteComponent(carpin4, std::shared_ptr<Journey::Material>(carpinMat), "carpincho", true);
        scene.AddEntity(nullptr, carpin4);

        mInnerVar = 0;
    }

    virtual void UserShutDown(Journey::Scene& scene) override {
        mInnerVar = 0;
    }

    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mInnerVar += deltaTime;
        carpin1->getTransform().SetTranslation(glm::vec3(25.0f, 15.0f +glm::sin(-1.57 + mInnerVar*0.8f)*5.0f , 0.5f));
        carpin2->getTransform().SetTranslation(glm::vec3(25.0f, 15.0f +glm::sin(+1.57 + mInnerVar*0.8f)*5.0f , 0.6f));
        carpin3->getTransform().SetTranslation(glm::vec3(25.0f+glm::sin(1.57+mInnerVar*0.5f)*2.0f , 15.0f +glm::sin(mInnerVar*0.5f)*6.0f , 0.6f));
        carpin4->getTransform().SetTranslation(glm::vec3(glm::sin(mInnerVar*0.5)*5.0f +25.0f,15.0f, 0.5f));
        carpin4->getTransform().SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(35.0f) + mInnerVar*0.5));
    }

private:
    float mInnerVar;
    std::shared_ptr<Journey::Entity> terrain;
    std::shared_ptr<Shiba> shiba;
    std::shared_ptr<Journey::FollowCamera> mainCamera;
    std::shared_ptr<Journey::Entity> carpin1, carpin2, carpin3, carpin4;
};

int main()
{   
    srand(time(NULL));
    LittleDemo littleDemo;
    Journey::Engine engine(littleDemo);
    engine.StartMainLoop();

    return 0;
}