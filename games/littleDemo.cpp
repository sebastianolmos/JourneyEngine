#include "JourneyEngine.hpp"

class LittleDemo : public Journey::Application {
    public:
        LittleDemo() = default;
        ~LittleDemo() = default;

        virtual void UserStartUp(Journey::Scene& scene) override {
            mInnerVar = 0.0f;
            mInnerVar++;

            mName = "LittleDemo";
            mWidth = 1200;
            mHeight = 800;
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