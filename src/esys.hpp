#pragma once
#include "pwra.h"
#include "mkr.h"

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
    rectangle source;

    void pos(float x, float y);
    void size(int x, int y);
    void draw(color color);
};
