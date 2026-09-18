#pragma once

class Scene{
public:
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual void Exit() {};
};
