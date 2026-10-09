#include "reg.hpp"
#include "mkgl.hpp"
#include "mktex.hpp"
#include "mktxt.hpp"
#include "pwra.h"
#include <string_view>

static std::vector<std::unique_ptr<Texture>> texmap = {};
static std::vector<std::unique_ptr<Sfx>> soundmap = {};
static std::vector<std::unique_ptr<Stream>> musicmap = {};
static std::vector<std::unique_ptr<Font>> fontmap = {};

void reg::init(){
    texmap.reserve(256);
    soundmap.reserve(256);
    musicmap.reserve(20);
    fontmap.reserve(5);
}

int reg::tex_register(std::string_view path){
    auto tex = std::make_unique<Texture>();

    *tex = mktex::LoadTextureSrc(path.data());
    texmap.push_back(std::move(tex));

    return texmap.size() - 1;
}

int reg::sfx_register(std::string_view path){
    auto sfx = std::make_unique<Sfx>();

    size_t size = 0;
    sfx->data = mkgl::loadBytes(path.data(), &size);

    pwra_load_sfx(sfx->data, size, &sfx->decoder, &sfx->source);

    soundmap.push_back(std::move(sfx));

    return soundmap.size() - 1;
}

int reg::stream_register(std::string_view path){
    auto music = std::make_unique<Stream>();

    pwra_load_stream(path.data(), &music->decoder, &music->source);

    musicmap.push_back(std::move(music));

    return musicmap.size() - 1;
}

int reg::font_register(std::string_view path){
    auto font = std::make_unique<Font>();
    *font = mktxt::LoadFont(path.data());

    fontmap.push_back(std::move(font));

    return fontmap.size() - 1;
}

Texture* reg::getTex(int id) { return texmap[id].get(); };
Sfx* reg::getSfx(int id) { return soundmap[id].get(); };
Stream* reg::getMusic(int id) { return musicmap[id].get(); };
Font* reg::getFont(int id) { return fontmap[id].get(); };

void reg::clear_tex(){
    for (auto& tex : texmap){
        if (tex) mktex::UnloadTexture(tex.get());
    }
    texmap.clear();
}

void reg::clear_sfx(){
    for (auto& sfx : soundmap){
        if (sfx) pwra_unload_sfx(sfx.get());
    }
    soundmap.clear();
}

void reg::clear_stream(){
    for (auto& music : musicmap){
        if (music) pwra_unload_stream(music.get());
    }
    musicmap.clear();
}

void reg::clear_font(){
    for (auto& font : fontmap){
        if (font) mktxt::UnloadFont(font.get());
    }
    fontmap.clear();
}

void reg::clear(){
    clear_tex();
    clear_sfx();
    clear_stream();
    clear_font();
}
