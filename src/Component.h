#pragma once

class Entity;

class Component {
    public:
    Entity* owner;
    virtual ~Component() {}
    virtual void Init() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    virtual void Terminate() {}
};