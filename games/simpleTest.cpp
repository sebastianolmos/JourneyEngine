#include "JourneyEngine.hpp"

void RegisterInputs(Journey::InputController& input) {
    input.RegisterGamepadAxis("MoveY", JOURNEY_GAMEPAD_AXIS_LEFT_Y);
    input.RegisterGamepadAction("Accelerate", JOURNEY_GAMEPAD_BUTTON_A);
}

void RegisterMeshes(Journey::MeshManager& manager) {
    manager.LoadPrimitiveMesh("stick", Journey::EPrimitiveMesh::Cylinder, true);
}

class SimpleTest : public Journey::Application {
public:
    SimpleTest() {
        mName = "Simple Test";
        mWidth = 900;
        mHeight = 900;
    }
    ~SimpleTest() = default;

    virtual void UserStartUp(Journey::Scene& scene) override {
        // User can use debug mode pressing Ctrl + D
        scene.DebugModeEnabled();

        // Registers
        RegisterInputs(scene.GetInputController());
        RegisterMeshes(Journey::MeshManager::getInstance());
        //Bindings
        scene.GetInputController().BindAxisMap("MoveY", [&](float dy) {this->moveZ(dy);} );
        scene.GetInputController().BindActionToggle("Accelerate", [&](bool b) {this->toggleAccel(b);} );

        stick = std::make_shared<Journey::Entity>();
        stick->getTransform().Set(glm::vec3(-0.0f, 0.0f, -0.2f),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(1.0f, 1.0f, 2.0f)
                                );
        Journey::FlatColoredMaterial* stickMat = new Journey::FlatColoredMaterial();
        stickMat->color = glm::vec3(0.0f, 0.0f, 0.8f);
        stickMat->kd = glm::vec3(0.5f, 0.5f, 0.5f);
        stickMat->ke = glm::vec3(0.4f, 0.4f, 0.4f);
        stickMat->ks = glm::vec3(0.2f, 0.2f, 0.2f);
        Journey::MeshManager::getInstance().AddMeshComponent(stick, std::shared_ptr<Journey::Material>(stickMat), "stick");
        scene.AddEntity(nullptr, stick);

        // Set Camera
        auto camera = std::make_shared<Journey::FollowCamera>(mWidth, mHeight);
        scene.GetCameraHandler().setCurrentCamera(camera);
        camera->setFollowPoint(stick->getTransform().GetLocalTranslation());
        camera->setDistanceRadius(5.0f);
        camera->setCenterHeight(0.0f);

         //Set light 
        scene.GetPointLight().setLightPos(glm::vec3(20.0f, -20.0f, 10.0f));
    }

    virtual void UserShutDown(Journey::Scene& scene) override {
    }

    virtual void UserUpdate(Journey::Scene& scene, float deltaTime) override {
        if (accelerating) 
            speed += accel * deltaTime;
        else 
            speed = (speed > 0.0f) ? speed - friction*deltaTime : 0.0f;
        rot += speed * deltaTime;
        zPos += zVel * deltaTime * 3.0f;

        stick->getTransform().SetTranslation(glm::vec3(0.0, 0.0, zPos));
        stick->getTransform().SetRotation(glm::vec3(0, glm::radians(90.0f), rot));
    }
    void moveZ(float dy) {
        zVel = (dy > 0.3 || dy < -0.3) ? -dy : 0.0f;
    }
    void toggleAccel(bool active) {
        accelerating = active;
    }

private:
    std::shared_ptr<Journey::Entity> stick;
    float rot = 0;
    float zPos, zVel = 0;
    float speed;
    bool accelerating = false;
    float friction = 15.0f;
    float accel = 5.0f;
};

int main()
{   
    SimpleTest simpleTest;
    Journey::Engine engine(simpleTest);
    engine.StartMainLoop();

    return 0;
}