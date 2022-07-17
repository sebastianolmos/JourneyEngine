#include "spotLight.hpp"

namespace Journey {
    
    void SpotLightComponent::turnOn(){
        mOn = true;
    }

    void SpotLightComponent::turnOff(){
        mOn = false;
    }

    bool SpotLightComponent::isOn(){
        return mOn;
    }

    glm::vec3 SpotLightComponent::getDirection() const{
        return mDirection;
    }

    const glm::vec3 SpotLightComponent::getAmbient() const{
        return mAmbient;
    }

    const glm::vec3 SpotLightComponent::getDiffuse() const{
        return mDiffuse;
    }

    const glm::vec3 SpotLightComponent::getSpecular() const{
        return mSpecular;
    }

    float SpotLightComponent::getConstantAttenuation() const{
        return mConstant;
    }

    float SpotLightComponent::getLinearAttenuation() const{
        return mLinear;
    }

    float SpotLightComponent::getQuadraticAttenuation() const{
        return mQuadratic;
    }
    
    float SpotLightComponent::getCutOff() const {
        return mCutOff;
    }

    float SpotLightComponent::getOuterCutOff() const {
        return mOuterCutOff;
    }

    void SpotLightComponent::setDirection(const glm::vec3 value) {
        mDirection = value;
    }

    void SpotLightComponent::setAmbient(const glm::vec3 value){
        mAmbient = value;
    }

    void SpotLightComponent::setDiffuse(const glm::vec3 value){
        mDiffuse = value;
    }

    void SpotLightComponent::setSpecular(const glm::vec3 value){
        mSpecular = value;
    }

    void SpotLightComponent::setConstantAttenuation(float value){
        mConstant = value;
    }

    void SpotLightComponent::setLinearAttenuation(float value){
        mLinear = value;
    }

    void SpotLightComponent::setQuadraticAttenuation(float value){
        mQuadratic = value;
    }

    void SpotLightComponent::setCutOff(float value) {
        mCutOff = value;
    }

    void SpotLightComponent::setOuterCutOff(float value) {
        mOuterCutOff = value;
    }   

}