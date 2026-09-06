#pragma once
#include "mkr.hpp"
#include <vector>

class anim{
public:
    static void PlayAnimation();
    static void SetAnimationFrames(const std::vector<Rectangle>& frames);
};
