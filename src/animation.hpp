#pragma once
#include "bml.h"
#include "tgt.h"
#include <vector>

struct Sheet{
    std::vector<rectangle> frames;
    int currFrame;
    float counter;
    double duration;
    bool end;
};

class anim{
public:
    static void PlayAnimation(Sheet& sheet, bool loop);
    static void RenderAnimation(Sheet& sheet, texture* tex, vec2 position, float size, color color);
};
