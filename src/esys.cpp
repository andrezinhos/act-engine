#include "esys.hpp"
#include "mkr.hpp"
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
    id = stack::PushSprite(path);
    source = stack::texmap[id].get();
}

void Sprite::pos(float x, float y){
    position.x = x;
    position.y = y;
}

void Sprite::size(int x, int y){
    if (source){
        source->width = x;
        source->height = y;
    }
}

void Sprite::draw(){
    if (source){
        Vec2 size = {
            static_cast<float>(source->width),
            static_cast<float>(source->height)
        };
        mkr::RenderTexture(source, position, size, White);
    }
}

void Sprite::draw_area(Rect& rec){
    if (source){
        Vec2 size = {
            static_cast<float>(source->width),
            static_cast<float>(source->height)
        };
        mkr::RenderTextureRec(source, rec.source, position, size, White);
    }
}

void Sound::load(cstr path){
    id = stack::PushSoundAudio(path);
    data = stack::soundmap[id].get();
}

void Sound::play(){
    if (data) PlaySfx(data);
}

void Sound::pitch(double amount){
    if (data) SetSfxPitch(data, amount);
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
    source = stack::fontmap[id].get();
}

void Text::pos(int x, int y){
    position.x = x;
    position.y = y;
}

void Text::spacing(double space){
    if (source)
        source->spacing = static_cast<float>(space);
    else
        dstate->dfont.spacing = static_cast<float>(space);
}

void Text::draw(cstr text, float size, Color color){
    if (source){
        mktxt::RenderTextEx(source, text, position, size, color);
    }
    else mktxt::RenderText(text, position, size, color);
}


void Anim2D::duration(double dur){
    source.duration = dur / 12.0;
}

void Anim2D::load(cstr path){
    id = stack::PushSprite(path);
    ref = stack::texmap[id].get();
}

void Anim2D::set_frames(const std::vector<std::vector<float>>& frames){
    source.frames.reserve(frames.size());
    for (auto& f : frames){
        source.frames.push_back({f[0], f[1], f[2], f[3]});
    }
}

void Anim2D::play(bool loop){
    anim::PlayAnimation(source, loop);
}

void Anim2D::pos(int x, int y){
    position.x = x;
    position.y = y;
}

void Anim2D::size(double size){
    size_val = size;
}

void Anim2D::draw(){
    if (ref) anim::RenderAnimation(source, ref, position, size_val, White);
}
