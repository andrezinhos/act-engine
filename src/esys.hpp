#pragma once
#include "mkr.hpp"

struct Sprite{
    Texture tex;
    Vec2 position;
    int id;

    void load(const char* path);
    void pos(float x, float y);
    void size(int x, int y);
};

class esys{
public:
    static void RenderSprite(int id, Sprite& sprite);
};
