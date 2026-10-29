#pragma once
#include "animation.hpp"
#include "mktxt.hpp"
#include "pwra.h"

struct Sound{
    Sfx* data;
    int id;

    void load(const char*  path);
    void play();
    void pitch(double amount);
};

struct Music{
    Stream* data;
    int id;

    void load(const char*  path);
    void play();
    void stop();
    void pause();
    void resume();
};

struct Rect{
    Rectangle source;

    void pos(float x, float y);
    void size(int x, int y);
    void draw(color color);
};

struct Sprite{
    Texture* source;
    Vec2 position;
    int id;

    void load(const char*  path);
    void pos(float x, float y);
    void size(int x, int y);
    void draw();
    void draw_area(Rect& rect);
};

struct Text{
    Font* source;
    Vec2 position;
    int id;

    void load(const char*  path);
    void pos(int x, int y);
    void spacing(double space);
    void draw(const char*  text, float size, color color);
};

struct Anim2D{
    Sheet source;
    Texture* ref;
    Vec2 position;
    float size_val;
    int id;
    bool playing;

    void duration(double dur);
    void load(const char*  path);
    void add_frame(int x, int y, int w, int h);
    void pos(int x, int y);
    void size(double size);
    void play(bool loop);
    void draw();
};
