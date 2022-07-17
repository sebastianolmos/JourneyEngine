#include "pointLight.hpp"

namespace Journey {
    
    void PointLightComponent::turnOn(){
        mOn = true;
    }

    void PointLightComponent::turnOff(){
        mOn = false;
    }

    bool PointLightComponent::isOn(){
        return mOn;
    }

    const glm::vec3 PointLightComponent::getAmbient() const{
        return mAmbient;
    }

    const glm::vec3 PointLightComponent::getDiffuse() const{
        return mDiffuse;
    }

    const glm::vec3 PointLightComponent::getSpecular() const{
        return mSpecular;
    }

    float PointLightComponent::getConstantAttenuation() const{
        return mConstant;
    }

    float PointLightComponent::getLinearAttenuation() const{
        return mLinear;
    }

    float PointLightComponent::getQuadraticAttenuation() const{
        return mQuadratic;
    }

    void PointLightComponent::setAmbient(const glm::vec3 value){
        mAmbient = value;
    }

    void PointLightComponent::setDiffuse(const glm::vec3 value){
        mDiffuse = value;
    }

    void PointLightComponent::setSpecular(const glm::vec3 value){
        mSpecular = value;
    }

    void PointLightComponent::setConstantAttenuation(float value){
        mConstant = value;
    }

    void PointLightComponent::setLinearAttenuation(float value){
        mLinear = value;
    }

    void PointLightComponent::setQuadraticAttenuation(float value){
        mQuadratic = value;
    }


}