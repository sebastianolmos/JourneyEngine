#pragma once
#include <memory>

#include "camera.hpp"

namespace Journey{

    class CameraHandler {
        public:
            CameraHandler() = default;
            
            glm::mat4 getProjection() {
                return mCurrentCamera->getProjection();
            }

            glm::mat4 getViewMatrix() {
                return mCurrentCamera->getViewMatrix();
            }

            glm::vec3 getViewPos(){
                return mCurrentCamera->getViewPos();
            }

            void setCurrentCamera(std::shared_ptr<Camera> newCamera) {
                mCurrentCamera = newCamera;
            }
        
        private:
            std::shared_ptr<Camera> mCurrentCamera;
    };

}