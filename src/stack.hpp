#pragma once
#include "mkgl.hpp"
#include <unordered_map>
#include "esys.hpp"

class stack{
public:
    static std::unordered_map<int, Texture> texmap;
    static std::unordered_map<int, Sound> soundmap;
    static std::unordered_map<int, Music> musicmap;
    static std::unordered_map<int, Font> fontmap;

    static int PushSprite(Texture& tex);
    static int PushSoundAudio(const char* path);
    static int PushMusicAudio(const char* path);
    static int PushFont(const char* path);
    static void UnloadAll();
};
