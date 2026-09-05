#pragma once
#include "mkgl.hpp"
#include "mkr.hpp"
#include "amk.hpp"
#include <vector>

struct Sound{
    std::vector<unsigned char> data;
    Decoder decoder;
    NodeSource source;
    int id;

    void load(const char* path);
    void play();
};

struct Music{
    Decoder decoder;
    NodeSource source;
    int id;

    void load(const char* path);
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

    void load(const char* path);
    void pos(float x, float y);
    void size(int x, int y);
    void draw();
    void draw_area(Rect& rect);
};

struct Text{
    Font font;
    Vec2 position;
    int id;

    void load(const char* path);
    void pos(int x, int y);
    void spacing(double space);
    void draw(const std::string& text, float size, Color color);
};
