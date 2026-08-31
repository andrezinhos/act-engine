#include "esys.hpp"
#include "stack.hpp"

void Sprite::load(const char* path){
    tex = mkr::LoadTextureSrc(path);
    id = stack::PushSprite(tex);
}

void Sprite::pos(float x, float y){
    position.x = x;
    position.y = y;
}

void Sprite::size(int x, int y){
    tex.width = x;
    tex.height = y;
}

void Sound::load(const char* path){
    id = stack::PushSoundAudio(path);
}

void Sound::play(int id){
    auto it = stack::soundmap.find(id);
    if (it != stack::soundmap.end()){
        amk::PlayAudioFile(it->second.decoder, it->second.source);
    }
}

void Music::load(const char* path){
    id = stack::PushMusicAudio(path);
}

void Music::play(int id){
    auto it = stack::soundmap.find(id);
    if (it != stack::soundmap.end()){
        amk::PlayAudioFile(it->second.decoder, it->second.source);
    }
}

void Music::stop(int id){
    //not implemented
}

void Music::pause(int id){
    //not implemented
}

void esys::RenderSprite(int id, Sprite& sprite){
    auto it = stack::texmap.find(id);
    if (it != stack::texmap.end()){
        Vec2 size = {(float)sprite.tex.width, (float)sprite.tex.height};
        mkr::RenderTexture(&it->second, sprite.position, size, White);
    }
}
