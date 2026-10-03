#include "stack.hpp"
#include "mktex.hpp"

std::unordered_map<int, std::unique_ptr<Texture>> stack::texmap;
int stack::sprite_count = 0;

std::unordered_map<int, std::unique_ptr<Sfx>> stack::soundmap;
int stack::sound_count = 0;

std::unordered_map<int, std::unique_ptr<Stream>> stack::musicmap;
int stack::music_count = 0;

std::unordered_map<int, std::unique_ptr<Font>> stack::fontmap;
int stack::font_count = 0;

int stack::PushSprite(cstr path){
    int id = sprite_count++;
    auto ref = std::make_unique<Texture>();
    *ref = mktex::LoadTextureSrc(path);
    texmap[id] = std::move(ref);
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

int stack::PushFont(cstr path){
    int id = font_count++;
    auto ref = std::make_unique<Font>();
    *ref = mktxt::LoadFont(path);
    fontmap[id] = std::move(ref);
    return id;
}

void stack::UnloadAll(){
    for (auto& [id, tex] : texmap){
        if (texmap.empty()) break;
        mktex::UnloadTexture(tex.release());
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
        mktxt::UnloadFont(font.release());
    }

    texmap.clear();
    soundmap.clear();
    musicmap.clear();
    fontmap.clear();

    // std::cout << "[INFO] STACK CLEAR\n";
}
