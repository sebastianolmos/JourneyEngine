#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "component.hpp"

namespace Journey {
    class RenderManager;

    class SpotLightComponent: public Component {
    public:
        SpotLightComponent() = default;
        virtual EComponentType GetType() override { return EComponentType::SpotLightComponent; }
        void turnOn();
        void turnOff();
        bool isOn();
        glm::vec3 getDirection() const;
        const glm::vec3 getAmbient() const;
        const glm::vec3 getDiffuse() const;
        const glm::vec3 getSpecular() const;
        float getConstantAttenuation() const;
        float getLinearAttenuation() const;
        float getQuadraticAttenuation() const;
        float getCutOff() const;
        float getOuterCutOff() const;
        void setDirection(const glm::vec3 value);
        void setAmbient(const glm::vec3 value);
        void setDiffuse(const glm::vec3 value);
        void setSpecular(const glm::vec3 value);
        void setConstantAttenuation(float value);
        void setLinearAttenuation(float value);
        void setQuadraticAttenuation(float value);
        void setCutOff(float value);
        void setOuterCutOff(float value);

    private:
        friend RenderManager;
        bool mOn = false;
        glm::vec3 mDirection = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 mAmbient;
        glm::vec3 mDiffuse;
        glm::vec3 mSpecular;
        float mConstant = 1.0f;
        float mLinear = 0.09f;
        float mQuadratic = 0.032f;
        float mCutOff;
        float mOuterCutOff;
    };
}