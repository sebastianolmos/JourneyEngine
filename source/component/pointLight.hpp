#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "component.hpp"

namespace Journey {
    class RenderManager;

    class PointLightComponent: public Component {
    public:
        PointLightComponent() = default;
        virtual EComponentType GetType() override { return EComponentType::PointLightComponent; }
        void turnOn();
        void turnOff();
        bool isOn();
        const glm::vec3 getAmbient() const;
        const glm::vec3 getDiffuse() const;
        const glm::vec3 getSpecular() const;
        float getConstantAttenuation() const;
        float getLinearAttenuation() const;
        float getQuadraticAttenuation() const;
        void setAmbient(const glm::vec3 value);
        void setDiffuse(const glm::vec3 value);
        void setSpecular(const glm::vec3 value);
        void setConstantAttenuation(float value);
        void setLinearAttenuation(float value);
        void setQuadraticAttenuation(float value);

    private:
        friend RenderManager;
        bool mOn = false;
        glm::vec3 mAmbient;
        glm::vec3 mDiffuse;
        glm::vec3 mSpecular;
        float mConstant = 1.0f;
        float mLinear = 0.09f;
        float mQuadratic = 0.032f;
    };
}