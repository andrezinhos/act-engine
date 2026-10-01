#include "stack.hpp"
#include "mktex.hpp"
#include <memory>

std::unordered_map<int, Texture> stack::texmap;
int stack::sprite_count = 0;

std::unordered_map<int, std::unique_ptr<Sfx>> stack::soundmap;
int stack::sound_count = 0;

std::unordered_map<int, std::unique_ptr<Stream>> stack::musicmap;
int stack::music_count = 0;

std::unordered_map<int, Font> stack::fontmap;
int stack::font_count = 0;

int stack::PushSprite(Texture* sprite){
    int id = sprite_count++;
    texmap[id] = *sprite;
    return id;
}

int stack::PushSoundAudio(const char* path){
    int id = sound_count++;
    size_t size = 0;
    auto ref = std::make_unique<Sfx>();
    ref->data = loadBytes(path, &size);
    LoadSfx(
        ref->data,
        size,
        &ref->decoder,
        &ref->source
    );

    soundmap[id] = std::move(ref);
    return id;
}

int stack::PushMusicAudio(const char* path){
    int id = music_count++;
    auto res = std::make_unique<Stream>();
    LoadStream(
        path,
        &res->decoder,
        &res->source
    );

    musicmap[id] = std::move(res);
    return id;
}

int stack::PushFont(const char *path){
    int id = font_count++;
    fontmap.emplace(id, mktxt::LoadFont(path));
    return id;
}

void stack::UnloadAll(){
    for (auto& [id, tex] : texmap){
        if (texmap.empty()) break;
        mktex::UnloadTexture(&tex);
    }
    for(auto& [id, sound] : soundmap){
        if (soundmap.empty()) break;
        UnloadSfx(sound.release());
    }
    for(auto& [id, music] : musicmap){
        if (musicmap.empty()) break;
        UnloadStream(music.release());
    }
    for(auto& [id, font] : fontmap){
        if (fontmap.empty()) break;
        mktxt::UnloadFont(&font);
    }

    texmap.clear();
    soundmap.clear();
    musicmap.clear();
    fontmap.clear();
    // printf("[INFO] STACK CLEAR\n");
}
