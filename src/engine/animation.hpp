#pragma once
#include "mkr.hpp"
#include <vector>

struct Sheet{
    std::vector<Rectangle> frames;
    int currFrame;
    double duration;
    double counter;
    bool end;
};

class anim{
public:
    static void PlayAnimation(Sheet& anim, bool loop);
    static std::vector<Rectangle> SetAnimationFrames(const std::vector<Rectangle>& frames);
    static void RenderAnimation(Sheet& sheet, Texture* tex, Vec2 position, float size, Color color);
};
