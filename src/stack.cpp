#include "stack.hpp"
#include "mktex.hpp"
#include "pwra.h"
#include <memory>

std::unordered_map<int, Texture> stack::texmap;
static int sprite_count = -1;

std::unordered_map<int, std::unique_ptr<Sfx>> stack::soundmap;
static int sound_count = -1;

std::unordered_map<int, std::unique_ptr<Stream>> stack::musicmap;
static int music_count = -1;

std::unordered_map<int, Font> stack::fontmap;
static int font_count = -1;

int stack::PushSprite(Texture& sprite){
    int id = sprite_count++;
    texmap[id] = sprite;
    return id;
}

int stack::PushSoundAudio(const char* path){
    int id = sound_count++;
    size_t size = 0;
    auto ref = std::make_unique<Sfx>();
    ref->data = loadBytes(path, &size);
    bool pass = LoadSfx(
        ref->data,
        size,
        &ref->decoder,
        &ref->source
    );

    if (!pass) UnloadSfx(ref.release());

    soundmap[id] = std::move(ref);
    return id;
}

int stack::PushMusicAudio(const char* path){
    int id = music_count++;
    auto res = std::make_unique<Stream>();
    bool pass = LoadStream(
        path,
        &res->decoder,
        &res->source
    );

    if (!pass) UnloadStream(res.release());

    musicmap[id] = std::move(res);
    return id;
}

int stack::PushFont(const char *path){
    int id = font_count++;
    fontmap.emplace(id, mktxt::LoadFont(path));
    return id;
}

void stack::UnloadAll(){
    for (auto const& [id, tex] : texmap){
        mktex::UnloadTexture(tex);
    }
    for(auto& [id, sound] : soundmap){
        UnloadSfx(sound.release());
    }
    for(auto& [id, music] : musicmap){
        UnloadStream(music.release());
    }
    for(auto& [id, font] : fontmap){
        mktxt::UnloadFont(font);
    }

    texmap.clear();
    soundmap.clear();
    musicmap.clear();
    fontmap.clear();
    // printf("[INFO] STACK CLEAR\n");
}
