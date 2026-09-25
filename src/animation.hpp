#pragma once
#include "mkr.hpp"
#include <vector>

struct Sheet{
    std::vector<Rectangle> frames;
    int currFrame;
    int counter;
    double duration;
    bool end;
};

class anim{
public:
    static void PlayAnimation(Sheet& sheet, bool loop);
    static void RenderAnimation(Sheet& sheet, Texture* tex, Vec2 position, float size, Color color);
};
