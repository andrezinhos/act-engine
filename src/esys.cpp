#include "esys.hpp"
#include "mkgl.hpp"
#include "mktex.hpp"
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

void Sprite::load(cstr path){
    source = mktex::LoadTextureSrc(path);
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

void Sound::load(cstr path){
    id = stack::PushSoundAudio(path);
    data = stack::soundmap[id].get();
}

void Sound::play(){
    if (data){
        PlaySfx(data);
    }
}

void Music::load(cstr path){
    id = stack::PushMusicAudio(path);
    data = stack::musicmap[id].get();
}

void Music::play(){
    if (data) PlayStream(data);
}

void Music::stop(){
    if (data) StopStream(data);
}

void Music::pause(){
    if (data) PauseStream(data);
}

void Music::resume(){
    if (data) ResumeStream(data);
}

void Text::load(cstr path){
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

void Text::draw(cstr text, float size, Color color){
    auto it = stack::fontmap.find(id);
    if (it != stack::fontmap.end()){
        mktxt::RenderTextEx(it->second, text, position, size, color);
    }
    else {
        mktxt::RenderText(text, position, size, color);
    }
}
