#pragma once
#include "mktypes.h"
#include "mktxt.hpp"
#include "pwra.h"
#include <unordered_map>
#include <memory>

class stack{
private:
    static int sprite_count;
    static int sound_count;
    static int music_count;
    static int font_count;
public:
    static std::unordered_map<int, Texture> texmap;
    static std::unordered_map<int, std::unique_ptr<Sfx>> soundmap;
    static std::unordered_map<int, std::unique_ptr<Stream>> musicmap;
    static std::unordered_map<int, Font> fontmap;

    static int PushSprite(Texture* tex);
    static int PushSoundAudio(const char* path);
    static int PushMusicAudio(const char* path);
    static int PushFont(const char* path);
    static void UnloadAll();
};
