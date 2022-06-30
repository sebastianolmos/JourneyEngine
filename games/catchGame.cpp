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
    manager.LoadPrimitiveMesh("coinMesh", Journey::EPrimitiveMesh::Cylinder, true);
    manager.LoadModelMesh("shibaMesh", "../../../assets/shiba/shiba.obj");
    manager.LoadModelMesh("cloudMesh", "../../../assets/cloud/cloudy.obj");
    manager.LoadModelMesh("barn", "../../../assets/farm/farm.fbx");
    manager.LoadModelMesh("bush", "../../../assets/bush/BUSH.obj");
    manager.LoadModelMesh("rabbit", "../../../assets/rabbit/Rabbit.FBX");
}

void RegisterTextures(Journey::TextureManager& manager) {
    manager.LoadModelTexture("carpincho", "../../../assets/sprites/carpincho.png");
}

void RegisterAudios(Journey::AudioManager& manager) {
    manager.LoadAudioBuffer("background", "../../../assets/audio/Guitar-Gentle.wav");
    manager.LoadAudioBuffer("bark", "../../../assets/audio/bark.wav");
    manager.LoadAudioBuffer("jumping", "../../../assets/audio/loop.wav");
    manager.LoadAudioBuffer("hit", "../../../assets/audio/sharp_punch.wav");
    manager.LoadAudioBuffer("coin", "../../../assets/audio/short_success_sound.wav");
    manager.LoadAudioBuffer("catched", "../../../assets/audio/tada.wav");
    
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
        mPos.y = -17.2f;
        // Add audio source
        scene.GetAudioManager().AddAudioSourceComponent(shared_from_this(), "bark", false, false);
    }
    void throwProjectile() {
        std::shared_ptr<ProjectileEntity> projectile = std::make_shared<ProjectileEntity>(
            mPos + mUp * mHeight * 2.0f, 
            1.5f, 
            8.0f + glm::length(mVel)*mRunSpeed, 
            glm::vec3(getFixedVel().x, getFixedVel().y, 0.0f), 
            0.2f, mAngleZ + glm::radians(90.0f)
        );
        mManager->AddEntity(nullptr, projectile);
        if (this->HasComponent(Journey::EComponentType::AudioSourceComponent)){
            Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(this->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
            audioSrc->setVolume(1.0f);
            audioSrc->play();
        }
    }
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
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
    }
private:
};

class PlayAndDelete: public Journey::Entity {
public:
    PlayAndDelete(glm::vec3 pos, std::string audioName, float volume) {
        getTransform().SetTranslation(pos);
        mAudio = audioName;
        mVolume = volume;
    }
    ~PlayAndDelete() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        scene.GetAudioManager().AddAudioSourceComponent(shared_from_this(), mAudio, false, false);
        if (this->HasComponent(Journey::EComponentType::AudioSourceComponent)){
            Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(this->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
            audioSrc->setVolume(mVolume);
            audioSrc->play();
        }
        
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        if (bShouldDelete) {
            //mManager->DeleteEntity(shared_from_this());
            return;
        }
        if (this->HasComponent(Journey::EComponentType::AudioSourceComponent)){
            Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(this->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
            if (!audioSrc->isPlaying()) {
                bShouldDelete = true;
            }
        }
    }

private:
    float mVolume;
    std::string mAudio;
    bool bShouldDelete = false;
    bool bPlaying = false;
};

class CoinEntity : public Journey::Entity {
public:
    CoinEntity(std::shared_ptr<Journey::Entity> dogRef, glm::vec3 pos, glm::vec3 vel) {
        mShibaRef = dogRef;
        getTransform().Set(pos, glm::vec3(glm::radians(90.0f), 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.1f));
        mPos = pos;
        mVel = vel;
    }
    ~CoinEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        Journey::PhongColoredMaterial* mat = new Journey::PhongColoredMaterial();
        mat->color = glm::vec3(0.828f, 0.683f, 0.214f);
        mat->kd = glm::vec3(0.1f, 0.1f, 0.1f);
        mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        mat->ks = glm::vec3(1.0f, 1.0f, 1.0f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "coinMesh");
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        if (mPos.z > mHeight) {
            mVel.z -= mGravity * deltaTime;
        } else {
            mVel.z = 0.0f;
            mPos.z = mHeight;
        }

        if (mVel.x != 0.0f && mVel.y != 0.0f) {
            mVel.x -= mFriction * deltaTime;
            mVel.y -= mFriction * deltaTime;
        } else {
            mVel.x = 0.0f;
            mVel.y = 0.0f;
        }
        mPos += mVel * deltaTime;
        mTheta += 2.0f * deltaTime;
        getTransform().SetTranslation(mPos);
        getTransform().SetRotation(glm::vec3(glm::radians(90.0f), 0.0f, mTheta));

        // Check collision with shiba
        if (glm::distance2(getTransform().GetLocalTranslation(), mShibaRef->getTransform().GetLocalTranslation()) < mRadius*mRadius) {
            std::shared_ptr<PlayAndDelete> audio = std::make_shared<PlayAndDelete>(getTransform().GetLocalTranslation(), "coin", 0.8);
            mManager->AddEntity(nullptr, audio);
            mManager->DeleteEntity(shared_from_this());
        }

    }

private:
    std::shared_ptr<Journey::Entity> mShibaRef;
    glm::vec3 mPos;
    glm::vec3 mVel;
    float mGravity = 3.0f;
    float mFriction = 0.1f;
    float mHeight = 1.0f;
    float mRadius = 0.8f;
    float mTheta = 0.0f;
};

class RabbitEntity : public Journey::Entity {
public:
    RabbitEntity(std::shared_ptr<Journey::Entity> dogRef, glm::vec3 pos, float size) {
        mShibaRef = dogRef;
        getTransform().Set(pos, glm::vec3(0.0f), glm::vec3(size));
        mPos.x = pos.x;
        mPos.y = pos.y;
        mHeight = pos.z;
    }
    ~RabbitEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        Journey::PhongTexturedMaterial* mat = new Journey::PhongTexturedMaterial();
        mat->kd = glm::vec3(0.6f, 0.6f, 0.6f);
        mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        mat->ks = glm::vec3(0.6f, 0.6f, 0.6f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "rabbit");
        scene.GetAudioManager().AddAudioSourceComponent(shared_from_this(), "jumping", false, true);
        if (this->HasComponent(Journey::EComponentType::AudioSourceComponent)){
            Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(this->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
            audioSrc->setVolume(0.8);
            audioSrc->play();
        }
        selectGoal();
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        mPos += mDir *speed * deltaTime;
        getTransform().SetTranslation(glm::vec3(mPos.x, mPos.y, mHeight));
        if (glm::distance2(mPos, mGoal) < 1.0f) {
            selectGoal();
        }

        // Check collision with shiba
        if (!mCatched && glm::distance2(mPos, glm::vec2(mShibaRef->getTransform().GetLocalTranslation().x, mShibaRef->getTransform().GetLocalTranslation().y)) < mRadius*mRadius) {
            std::shared_ptr<PlayAndDelete> audio = std::make_shared<PlayAndDelete>(getTransform().GetLocalTranslation(), "catched", 0.8);
            if (this->HasComponent(Journey::EComponentType::AudioSourceComponent)){
                Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(this->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
                audioSrc->stop();
            }
            mCatched = true;
            mManager->AddEntity(nullptr, audio);
            speed = 0.0f;
            int n = 20;
            float pi = glm::radians(180.0f);
            float angle = 2*pi/n;
            float speed = 2.0f;
            for (int i = 0; i < n; i++) {
                std::shared_ptr<CoinEntity> coin = std::make_shared<CoinEntity>(mShibaRef, getTransform().GetLocalTranslation(),
                glm::vec3(speed*glm::cos(angle*(float)i),speed*glm::sin(angle*(float)i),speed*1.6f));
                mManager->AddEntity(nullptr, coin);
            }
        }
    }

    void selectGoal() {
        bool unselect = true;
        while (unselect) {
            float ranX = (((rand() / static_cast<float>(RAND_MAX)) * 2.0f ) -1.0f ) * 50.0f;
            float ranY = -12.5 + (((rand() / static_cast<float>(RAND_MAX)) * 2.0f ) -1.0f ) * 35.0f;
            if (ranX > mLeft && ranX < mRight && ranY < mTop && ranY > mBottom) {
                unselect = false;
                mGoal = glm::vec2(ranX, ranY);
                mDir = glm::normalize(mGoal - mPos);
            }
        }
    }

private:
    std::shared_ptr<Journey::Entity> mShibaRef;
    float mLeft = -50.0f;
    float mRight = 50.0f;
    float mTop = 20.0f;
    float mBottom = -35.0f;
    glm::vec2 mGoal;
    glm::vec2 mPos;
    glm::vec2 mDir;
    float mHeight;
    float speed = 7.0f;
    float mRadius = 1.2f;
    bool mCatched =false;

};

class BushEntity : public Journey::Entity {
public:
    BushEntity(glm::vec3 pos, float size) {
         getTransform().Set(pos, glm::vec3(0.0f), glm::vec3(size));
    }
    ~BushEntity() = default;
    virtual void UserStartUp(Journey::Scene& scene) override {
        Journey::PhongTexturedMaterial* mat = new Journey::PhongTexturedMaterial();
        mat->kd = glm::vec3(0.8f, 0.8f, 0.8f);
        mat->ke = glm::vec3(0.5f, 0.5f, 0.5f);
        mat->ks = glm::vec3(0.6f, 0.6f, 0.6f);
        Journey::MeshManager::getInstance().AddMeshComponent(shared_from_this(), std::shared_ptr<Journey::Material>(mat), "bush");
    }
    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
    }

private:
};


class CatchGame : public Journey::Application {
public:
    CatchGame() {
        mName = "Catch Game! OwO";
        mWidth = 1200;
        mHeight = 800;
    }
    ~CatchGame() = default;

    virtual void UserStartUp(Journey::Scene& scene) override {
        // User can use debug mode pressing Ctrl + D
        scene.DebugModeEnabled();

        // Registers
        RegisterInputs(scene.GetInputController());
        RegisterMeshes(Journey::MeshManager::getInstance());
        RegisterTextures(Journey::TextureManager::getInstance());
        RegisterAudios(scene.GetAudioManager());

        auto& meshManager = Journey::MeshManager::getInstance();
        // Floor Terrain entity
        barn = std::make_shared<Journey::Entity>();
        barn->getTransform().Set(glm::vec3(-0.0f, -47.5f, -0.2f),
                                glm::vec3(0.0f, 0.0f, glm::radians(180.0f)),
                                glm::vec3(5.0f, 5.0f, 5.0f)
                                );
        Journey::PhongTexturedMaterial* barnMat = new Journey::PhongTexturedMaterial();
        barnMat->kd = glm::vec3(0.5f, 0.5f, 0.5f);
        barnMat->ke = glm::vec3(0.4f, 0.4f, 0.4f);
        barnMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);
        meshManager.AddMeshComponent(barn, std::shared_ptr<Journey::Material>(barnMat), "barn");
        scene.AddEntity(nullptr, barn);


        // Set Camera
        mainCamera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
        scene.GetCameraHandler().setCurrentCamera(mainCamera);
        mainCamera->setFov(45.0f);
        mainCamera->setFar(120.0f);

        // Add audio listener
        scene.GetAudioManager().AddListener(mainCamera->getViewPos(), 1.0f);

        // Add background music
        scene.GetAudioManager().AddAudioSourceComponent(barn, "background", true, true);
        if (barn->HasComponent(Journey::EComponentType::AudioSourceComponent)){
            Journey::AudioSourceComponent* audioSrc = dynamic_cast<Journey::AudioSourceComponent*>(barn->GetComponent(Journey::EComponentType::AudioSourceComponent).get());
            audioSrc->setVolume(0.15f);
            audioSrc->play();
        }

        //Set light 
        scene.GetPointLight().setLightPos(glm::vec3(20.0f, -20.0f, 10.0f));

        // Entity creation - method 2
        shiba = std::make_shared<Shiba>(mainCamera);
        scene.AddEntity(nullptr, shiba);

        // Add the initial coins
        for (int i = 0 ; i < 5; i++) {
            for (int j = 0; j < 3 ; j ++) {
                std::shared_ptr<CoinEntity> coin = std::make_shared<CoinEntity>(shiba,
                    glm::vec3(25.0f + 2.0f*j, 10.0f - 5.0f*i, 15.0f), 
                    glm::vec3(0.0f));
                scene.AddEntity(nullptr, coin);
            }
        }

        // Add the rabbit
        std::shared_ptr<RabbitEntity> rabbit = std::make_shared<RabbitEntity>(shiba,
            glm::vec3(0, 20.0, -0.5f), 0.05f);
        scene.AddEntity(nullptr, rabbit);

        //Add the bushes
        std::shared_ptr<BushEntity> bush1 = std::make_shared<BushEntity>(glm::vec3(-15.0f, 0.0f, -7.0f), 7.0f);
        scene.AddEntity(nullptr, bush1);
        std::shared_ptr<BushEntity> bush2 = std::make_shared<BushEntity>(glm::vec3(-35.0f, 0.0f, -7.0f),  7.0f);
        scene.AddEntity(nullptr, bush2);
        std::shared_ptr<BushEntity> bush3 = std::make_shared<BushEntity>(glm::vec3(-25.0f, -15.0f, -7.0f),  7.0f);
        scene.AddEntity(nullptr, bush3);
        std::shared_ptr<BushEntity> bush4 = std::make_shared<BushEntity>(glm::vec3(-25.0f,  15.0f, -7.0),  7.0f);
        scene.AddEntity(nullptr, bush4);

        // Add the sun 
        auto sun = std::make_shared<SunEntity>();
        scene.AddEntity(nullptr, sun);

        // Add the plane thta represnt the grass
        auto grass = std::make_shared<Journey::Entity>();
        grass->getTransform().Set(glm::vec3(0.0f, -12.5f, -0.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(100.0f, 98.0f, 0.3f)
                                );
        Journey::PhongColoredMaterial* grassMat = new Journey::PhongColoredMaterial();
        grassMat->color = glm::vec3(0.3f, 1.0f, 0.01f);
        grassMat->kd = glm::vec3(0.3f, 0.3f, 0.4f);
        grassMat->ke = glm::vec3(0.2f, 0.2f, 0.2f);
        grassMat->ks = glm::vec3(0.0f, 0.0f, 0.0f);
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
    std::shared_ptr<Journey::Entity> barn;
    std::shared_ptr<Shiba> shiba;
    std::shared_ptr<Journey::FollowCamera> mainCamera;
    std::shared_ptr<Journey::Entity> carpin1, carpin2, carpin3, carpin4;
};

int main()
{   
    srand(time(NULL));
    CatchGame catchGame;
    Journey::Engine engine(catchGame);
    engine.StartMainLoop();

    return 0;
}