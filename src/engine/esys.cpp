#include "esys.hpp"
#include "stack.hpp"

void Rect::pos(float x, float y){
    source.x = x;
    source.y = y;
}

void Rect::size(int x, int y){
    source.width = x;
    source.height = y;
}

void Rect::draw(Color color){
    mkr::RenderRectangle(
        {(float)source.x, (float)source.y}, 
        {(float)source.width, (float)source.height}, 
        color
    );
}

void Sprite::load(const char* path){
    source = mkr::LoadTextureSrc(path);
    id = stack::PushSprite(source);
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

void Sprite::draw_area(Rect& rec){
    auto it = stack::texmap.find(id);
    if (it != stack::texmap.end()){
        Vec2 size = {
            static_cast<float>(it->second.width), 
            static_cast<float>(it->second.height)
        };
        mkr::RenderTextureRec(&it->second, rec.source, position, size, White);
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

void Text::load(const char* path){
    id = stack::PushFont(path);
}

void Text::pos(int x, int y){      
    position.x = x;
    position.y = y;
}

void Text::spacing(double space){
    auto it = stack::fontmap.find(id);
    if (it != stack::fontmap.end())
        it->second.spacing = static_cast<float>(space);
    else 
        mkr::state.dfont.spacing = static_cast<float>(space);
}

void Text::draw(const std::string& text, float size, Color color){
    auto it = stack::fontmap.find(id);
    if (it != stack::fontmap.end()){
        mktxt::RenderTextEx(it->second, text, position, size, color);
    }
    else {
        mktxt::RenderText(text, position, size, color);
    }
}
