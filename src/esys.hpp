#pragma once
#include "mkr.hpp"
#include "amk.hpp"

struct Sound{
    Decoder decoder;
    NodeSource source;
    int id;

    void load(const char* path);
    void play(int id);
};

struct Music{
    Decoder decoder;
    NodeSource source;
    int id;

    void load(const char* path);
    void play(int id);
    void stop(int id);
    void pause(int id);
};

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
