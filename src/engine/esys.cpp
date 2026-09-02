#include "esys.hpp"
#include "stack.hpp"

void Rect::pos(float x, float y){
    position.x = x;
    position.y = y;
}

void Rect::size(int x, int y){
    width = x;
    height = y;
}

void Rect::draw(Color color){
    mkr::RenderRectangle(
        position, 
        {(float)width, (float)height}, 
        color
    );
}

void Sprite::load(const char* path){
    tex = mkr::LoadTextureSrc(path);
    id = stack::PushSprite(tex);
}

void Sprite::pos(float x, float y){
    position.x = x;
    position.y = y;
}

void Sprite::size(int x, int y){
    auto it = stack::texmap.find(id);
    if (it != stack::texmap.end()){
        it->second.width = x;
        it->second.height = y;
    }
}

void Sound::load(const char* path){
    id = stack::PushSoundAudio(path);
}

void Sound::play(){
    auto it = stack::soundmap.find(stack::soundmap[id].id);
    if (it != stack::soundmap.end()){
        amk::PlayAudioFile(it->second.decoder, it->second.source);
    }
}

void Music::load(const char* path){
    id = stack::PushMusicAudio(path);
}

void Music::play(){
    auto it = stack::musicmap.find(stack::musicmap[id].id);
    if (it != stack::musicmap.end()){
        amk::PlayAudioFile(it->second.decoder, it->second.source);
    }
}

void Music::stop(){
    auto it = stack::musicmap.find(stack::musicmap[id].id);
    if (it != stack::musicmap.end()){
        amk::StopAudioFile(it->second.decoder, it->second.source);
    }
}

void Music::pause(){
    auto it = stack::musicmap.find(stack::musicmap[id].id);
    if (it != stack::musicmap.end()){
        amk::PauseAudioFile(it->second.source);
    }
}

void Music::resume(){
    auto it = stack::musicmap.find(stack::musicmap[id].id);
    if (it != stack::musicmap.end()){
        amk::ResumeAudioFile(it->second.source);
    }
}

void Sprite::draw(){
    auto it = stack::texmap.find(id);
    if (it != stack::texmap.end()){
        Vec2 size = {
            static_cast<float>(it->second.width), 
            static_cast<float>(it->second.height)
        };
        mkr::RenderTexture(&it->second, position, size, White);
    }
}
