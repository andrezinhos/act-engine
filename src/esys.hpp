#pragma once
#include "animation.hpp"
#include "mkgl.hpp"
#include "mkr.hpp"
#include "amk.h"

struct Sound{
    unsigned char* data;
    Decoder decoder;
    NodeSource source;
    int id;

    void load(cstr path);
    void play();
};

struct Music{
    Decoder decoder;
    NodeSource source;
    int id;

    void load(cstr path);
    void play();
    void stop();
    void pause();
    void resume();
};

struct Rect{
    Rectangle source;

    void pos(float x, float y);
    void size(int x, int y);
    void draw(Color color);
};

struct Sprite{
    Texture source;
    Vec2 position;
    int id;

    void load(cstr path);
    void pos(float x, float y);
    void size(int x, int y);
    void draw();
    void draw_area(Rect& rect);
};

struct Text{
    Font font;
    Vec2 position;
    int id;

    void load(cstr path);
    void pos(int x, int y);
    void spacing(double space);
    void draw(cstr text, float size, Color color);
};

struct Anim2D{
    Sheet source;
};
