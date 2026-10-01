#pragma once
#include "mktypes.h"
#include "mkmath.hpp"
#include <vector>

struct Sheet{
    std::vector<Rectangle> frames;
    int currFrame;
    float counter;
    double duration;
    bool end;
};

class anim{
public:
    static void PlayAnimation(Sheet& sheet, bool loop);
    static void RenderAnimation(Sheet& sheet, Texture* tex, Vec2 position, float size, Color color);
};
