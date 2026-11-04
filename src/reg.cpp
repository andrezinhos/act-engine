#include "mktex.hpp"
#include "mktxt.hpp"
#include "pwra.h"
#include "core.hpp"
#include <stdio.h>
#include <string_view>

static std::vector<std::unique_ptr<Texture>> texmap = {};
static std::vector<std::unique_ptr<Sfx>> soundmap = {};
static std::vector<std::unique_ptr<Stream>> musicmap = {};
static std::vector<std::unique_ptr<Font>> fontmap = {};

void engine::reg::init(){
    texmap.reserve(256);
    soundmap.reserve(32);
    musicmap.reserve(20);
    fontmap.reserve(5);
}

int engine::reg::tex_register(std::string_view path){
    auto tex = std::make_unique<Texture>();

    engine::image image;
    image.load(path);

    *tex = mktex::load_tex_src(image.data(), image.width(), image.height());
    texmap.push_back(std::move(tex));

    return texmap.size() - 1;
}

int engine::reg::sfx_register(std::string_view path){
    auto sfx = std::make_unique<Sfx>();

    pwra_load_sfx(path.data(), sfx.get());
    soundmap.push_back(std::move(sfx));

    return soundmap.size() - 1;
}

int engine::reg::stream_register(std::string_view path){
    auto music = std::make_unique<Stream>();

    pwra_load_stream(path.data(), music.get());
    musicmap.push_back(std::move(music));

    return musicmap.size() - 1;
}

int engine::reg::font_register(std::string_view path){
    auto font = std::make_unique<Font>();
    *font = mktxt::load_font(path.data());

    fontmap.push_back(std::move(font));

    return fontmap.size() - 1;
}

Texture* engine::reg::get_tex(int id) {
    return texmap[id].get();
};

Sfx* engine::reg::get_sfx(int id) {
    return soundmap[id].get();
};

Stream* engine::reg::get_music(int id) {
    return musicmap[id].get();
};

Font* engine::reg::get_font(int id) {
    return fontmap[id].get();
};

void engine::reg::clear_tex(){
    for (auto& tex : texmap){
        if (tex) mktex::unload_tex(tex.get());
    }
    texmap.clear();
}

void engine::reg::clear_sfx(){
    for (auto& sfx : soundmap){
        if (sfx) pwra_unload_sfx(sfx.get());
    }
    soundmap.clear();
}

void engine::reg::clear_stream(){
    for (auto& music : musicmap){
        if (music) pwra_unload_stream(music.get());
    }
    musicmap.clear();
}

void engine::reg::clear_font(){
    for (auto& font : fontmap){
        if (font) mktxt::unload_font(font.get());
    }
    fontmap.clear();
}

void engine::reg::clear(){
    clear_tex();
    clear_sfx();
    clear_stream();
    clear_font();
}
