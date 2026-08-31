#include "stack.hpp"
#include "esys.hpp"
#include "mkr.hpp"

std::unordered_map<int, Texture> stack::texmap;
static int sprite_count = 0;

std::unordered_map<int, Sound> stack::soundmap;
static int sound_count = 0;

std::unordered_map<int, Music> stack::musicmap;
static int music_count = 0;

int stack::PushSprite(Texture& sprite){
    int id = sprite_count++;
    texmap.emplace(id, sprite);
    return id;
}

int stack::PushSoundAudio(const char* path){
    int id = sound_count++;
    amk::LoadAudioFile(path, soundmap[id].decoder, soundmap[id].source);
    return id;
}

int stack::PushMusicAudio(const char* path){
    int id = sound_count++;
    amk::LoadAudioFile(path, musicmap[id].decoder, musicmap[id].source);
    return id;
}

void stack::UnloadAll(){
    for (auto const& [id, tex] : texmap){
        mkr::UnloadTexture(tex);
    }
    for(auto& [id, sound] : soundmap){
        amk::UnloadAudio(sound.decoder, sound.source);
    }
    for(auto& [id, music] : musicmap){
        amk::UnloadAudio(music.decoder, music.source);
    }

    texmap.clear();
    soundmap.clear();
    musicmap.clear();
    // printf("[INFO] STACK CLEAR\n");
}
