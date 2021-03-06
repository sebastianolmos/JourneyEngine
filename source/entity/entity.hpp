#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "../component/component.hpp"
#include "../component/transform.hpp"

namespace Journey{
    class Scene;
    class EntityManager;
    class TextureManager;
    class MeshManager;
    class AudioManager;
    class SkeletalManager;
    enum class EState {
        Active,
        PendingDestroy
    };
    class Entity : public std::enable_shared_from_this<Entity> {
        public:

            Transform& getTransform() {return mTransform;}
            
            Entity() : mCurrentState(EState::Active) {}
            virtual ~Entity() {};

            void StartUp(Scene& scene, std::shared_ptr<EntityManager> manager)  
            { 
                mManager = manager;
                UserStartUp(scene);
            };

            void Update(Scene& scene, float deltaTime) {
                if (mCurrentState == EState::PendingDestroy)
                    return;
                UserUpdate(scene, deltaTime);
            }

            virtual void UserUpdate(Scene& scene, float deltaTime) {};
            virtual void UserStartUp(Scene& scene){};
            const EState GetState() const { return mCurrentState; }
            bool HasComponent(EComponentType component) {return mComponents.count(component) != 0;}
            
            std::shared_ptr<Component> GetComponent(EComponentType component) {
                if (HasComponent(component)) 
                    return mComponents[component];
                else
                    nullptr;
            }


        protected:

            //Entity(const Entity&) = delete;
            //Entity& operator=(const Entity&) = delete;
            //Entity(Entity&&) = default;
            //Entity& operator=(Entity&&) = default;

            friend EntityManager;
            friend TextureManager;
            friend MeshManager;
            friend AudioManager;
            friend Scene;
            friend SkeletalManager;

            void ShutDown() noexcept {mCurrentState = EState::PendingDestroy; }

            Transform mTransform;
            EState mCurrentState;

            std::unordered_map<EComponentType, std::shared_ptr<Component>> mComponents;
            int mId;
            std::unordered_map<int, std::shared_ptr<Entity>> mChildren;
            std::shared_ptr<Entity> mParent;
            std::shared_ptr<EntityManager> mManager;

    };

}