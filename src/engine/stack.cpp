#include "stack.hpp"
#include "esys.hpp"
#include "mkr.hpp"
#include "amk.h"

std::unordered_map<int, Texture> stack::texmap;
static int sprite_count = 0;

std::unordered_map<int, Sound> stack::soundmap;
static int sound_count = 0;

std::unordered_map<int, Music> stack::musicmap;
static int music_count = 0;

std::unordered_map<int, Font> stack::fontmap;
static int font_count = 0;

int stack::PushSprite(Texture& sprite){
    int id = sprite_count++;
    texmap[id] = sprite;
    return id;
}

int stack::PushSoundAudio(const char* path){
    int id = sound_count++;
    size_t size = 0;
    soundmap[id].data = loadBytes(path, &size);
    LoadSoundAudioFile(
        soundmap[id].data,
        size,
        &soundmap[id].decoder,
        &soundmap[id].source
    );
    return id;
}

int stack::PushMusicAudio(const char* path){
    int id = music_count++;
    LoadMusicAudioFile(
        path,
        &musicmap[id].decoder,
        &musicmap[id].source
    );
    return id;
}

int stack::PushFont(const char *path){
    int id = font_count++;
    fontmap.emplace(id, mktxt::LoadFont(path));
    return id;
}

void stack::UnloadAll(){
    for (auto const& [id, tex] : texmap){
        mkr::UnloadTexture(tex);
    }
    for(auto& [id, sound] : soundmap){
        UnloadSoundAudio(sound.data, &sound.decoder, &sound.source);
    }
    for(auto& [id, music] : musicmap){
        UnloadAudioFile(&music.decoder, &music.source);
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
