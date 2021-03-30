#pragma once

#include <vector>
#include<string>
#include <map>
#include "./Constants.h"
#include "./EntityManager.h"
#include "./Component.h"

class Component;
class EntityManager;

class Entity {
    private:
        EntityManager& manager;
        bool isActive;
        bool isDestroyed;
        Height height;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> componentTypeMap;
    public:
        std::string name;
        EntityType type;
        LayerType layer;
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, std::string name, EntityType type, LayerType layer);
        void Update(float deltaTime);
        void Render();
        void Terminate();
        Height GetHeight();
        void SetHeight(Height height);
        void SetActive(bool state);
        bool IsActive() const;
        bool IsDestroyed() const;

        template<typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            newComponent->owner = this;
            components.emplace_back(newComponent);
            componentTypeMap[&typeid(*newComponent)] = newComponent;
            newComponent->Init();
            return *newComponent;
        }
        
        template<typename T>
        T* GetComponent() {
            return static_cast<T*>(componentTypeMap[&typeid(T)]);
        }

        template<typename T>
        bool HasComponent() const {
            return componentTypeMap.count(&typeid(T));
        }
};