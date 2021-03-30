#pragma once

#include "./Entity.h"
#include "./Component.h"

#include <SDL2/SDL.h>

class ColliderComponent;

class EntityManager {
    private:
        Entity* player;
        std::vector<Entity*> entities;
        std::vector<ColliderComponent*> colliders;
    public:
        void ClearData();
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities() const;
        Entity& AddEntity(std::string entityName, EntityType type, LayerType layer);
        void AddCollider(ColliderComponent *collider);
        std::vector<Entity*>GetEntities() const;
        std::vector<ColliderComponent*>GetColliders() const;
        std::vector<Entity*>GetEntitiesByLayer(LayerType layer) const;
        void SetPlayer(std::string name) { player = GetEntityByName(name); } ;
        Entity* GetPlayer() const { return player; } ;
        Entity* GetEntityByName(std::string name) const;
        unsigned int GetEntityCount();
        unsigned int GetColliderCount();
        void CheckCollisions() const;
        void DestroyEntity(std::string name);
        void EraseDestroyedEntities();
};