#pragma once
#include <pwra.h>
#include "mktxt.hpp"
#include <unordered_map>

class stack{
private:
    static int sprite_count;
    static int sound_count;
    static int music_count;
    static int font_count;
public:
    static std::unordered_map<int, std::unique_ptr<Texture>> texmap;
    static std::unordered_map<int, std::unique_ptr<Sfx>> soundmap;
    static std::unordered_map<int, std::unique_ptr<Stream>> musicmap;
    static std::unordered_map<int, std::unique_ptr<Font>> fontmap;

    static int PushSprite(cstr path);
    static int PushSoundAudio(cstr path);
    static int PushMusicAudio(cstr path);
    static int PushFont(cstr path);
    static void UnloadAll();
};
