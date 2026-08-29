#pragma once
#include "mkr/mkgl.hpp"
#include <unordered_map>

class stack{
public:
    static std::unordered_map<int, Texture> texmap;
    static int PushSprite(Texture& tex);
    static void UnloadAll();
};
