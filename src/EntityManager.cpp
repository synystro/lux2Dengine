#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData() {
    for(auto& entity : entities) {
        entity->Terminate();
    }
}

void EntityManager::DestroyEntity(std::string name) {
    for(auto& entity : entities) {
        if(entity->name == name) {
            entity->Terminate();
        }
    }
}

void EntityManager::EraseDestroyedEntities() {
    for(int i = 0; i < entities.size(); i++) {
        if(entities[i]->IsDestroyed()) {
            entities.erase(entities.begin() + i);
        }
    }
}

bool EntityManager::HasNoEntities() const {
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
    for(auto& entity : entities) {
        if(entity->IsActive())
            entity->Update(deltaTime);
    }
    EraseDestroyedEntities();
}

void EntityManager::Render() {
    for(int layerIndex = 0; layerIndex < NUM_LAYERS; layerIndex++) {
        for(auto& entity : GetEntitiesByLayer(static_cast<LayerType>(layerIndex))) {
            if(entity->IsActive())
                entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, EntityType type, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, type, layer);
    entities.emplace_back(entity);
    return *entity;
}

void EntityManager::AddCollider(ColliderComponent *collider) {
    colliders.emplace_back(collider);
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

std::vector<ColliderComponent*>EntityManager::GetColliders() const {
    return colliders;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> entitiesFound;
    for(auto& entity : entities) {
        if(entity->layer == layer) {
            entitiesFound.emplace_back(entity);
        }
    }
    return entitiesFound;
}

Entity* EntityManager::GetEntityByName(std::string name) const {
    for(auto& entity : entities) {
        if(entity->name == name)
            return entity;
    }
    return nullptr;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

unsigned int EntityManager::GetColliderCount() {
    return colliders.size();
}

void EntityManager::CheckCollisions() const {
    for(int i = 0; i < colliders.size(); i++) {
        for(int j = i + 1; j < colliders.size(); j++) {
            if(!colliders[i]->owner->IsActive() || !colliders[j]->owner->IsActive())
                return;
            if(Collision::CheckRectCollision(colliders[i]->collider, colliders[j]->collider)) {
                colliders[i]->OnCollision(colliders[j]);
            }
        }
    }
}