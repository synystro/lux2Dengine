#include "./Entity.h"

Entity::Entity(EntityManager& manager): manager(manager) {
    this->isActive = true;
    this->isDestroyed = false;
}
Entity::Entity(EntityManager& manager, std::string name, EntityType type, LayerType layer): manager(manager), name(name), type(type), layer(layer) {
    this->isActive = true;
    this->isDestroyed = false;

    if(type == EntityType::GROUND) {
        height = Height::GROUND;
    } else if(type == EntityType::FLYING) {
        height = Height::FLYING;
    } else {
        height = Height::FREEPATHING;
    }
}

void Entity::Update(float deltaTime) {
    for(auto& component : components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for(auto& component : components) {
        component->Render();
    }
}

void Entity::Terminate() {
    this->isDestroyed = true;
}

Height Entity::GetHeight() {
    return this->height;
}

void Entity::SetHeight(Height height) {
    this->height = height;
}

void Entity::SetActive(bool state) {
    this->isActive = state;
}

bool Entity::IsActive() const {
    return this->isActive;
}

bool Entity::IsDestroyed() const {
    return this->isDestroyed;
}