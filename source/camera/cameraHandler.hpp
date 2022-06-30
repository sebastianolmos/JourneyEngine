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

            glm::vec3 getFront()
            {
                return mCurrentCamera->getFront();
            }

            glm::vec3 getUp()
            {
                return mCurrentCamera->getUp();
            }

            void setCurrentCamera(std::shared_ptr<Camera> newCamera) {
                mCurrentCamera = newCamera;
            }

            std::shared_ptr<Camera> getCurrentCamera() {
                return mCurrentCamera;
            }
        
        private:
            std::shared_ptr<Camera> mCurrentCamera;
    };

}