#pragma once
#include "entity.hpp"

#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>
#include "../component/transform.hpp"
#include "../rendering/renderManager.hpp"


namespace Journey {

    class Scene;
    class EntityManager {
        public:
            EntityManager();

            void StartUp();
            void ShutDown();

            void UpdateEntities(Scene& scene, float deltaTime);
            void AddEntity(std::shared_ptr<Entity> parentEntity, std::shared_ptr<Entity> newEntity);


        private:
            int mEntitiesCount;
            std::unordered_map<int, std::shared_ptr<Entity>> mEntities;

            void UpdateEntity(Scene& scene, std::shared_ptr<Entity> entity, glm::mat4 parentTransform, float deltaTime);
            

    };

}