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

void esys::RenderSprite(int id, Sprite& sprite){
    auto it = stack::texmap.find(id);
    if (it != stack::texmap.end()){
        Vec2 size = {(float)sprite.tex.width, (float)sprite.tex.height};
        mkr::RenderTexture(&it->second, sprite.position, size, White);
    }
}
