#include "stack.hpp"
#include "mkgl.hpp"
#include "mktex.hpp"
#include "mktxt.hpp"
#include "pwra.h"

std::vector<std::unique_ptr<Texture>> texmap = {};
std::vector<std::unique_ptr<Sfx>> soundmap = {};
std::vector<std::unique_ptr<Stream>> musicmap = {};
std::vector<std::unique_ptr<Font>> fontmap = {};

void reg::init(){
    texmap.reserve(1024);
    soundmap.reserve(1024);
    musicmap.reserve(1024);
    fontmap.reserve(1024);
}

int reg::tex_register(const char *path){
    auto tex = std::make_unique<Texture>();
    *tex = mktex::LoadTextureSrc(path);

    texmap.push_back(std::move(tex));

    return texmap.size() - 1;
}

int reg::sfx_register(const char *path){
    auto sfx = std::make_unique<Sfx>();

    size_t size = 0;
    sfx->data = mkgl::loadBytes(path, &size);

    pwra_load_sfx(sfx->data, size, &sfx->decoder, &sfx->source);

    soundmap.push_back(std::move(sfx));

    return soundmap.size() - 1;
}

int reg::stream_register(const char *path){
    auto music = std::make_unique<Stream>();

    pwra_load_stream(path, &music->decoder, &music->source);

    musicmap.push_back(std::move(music));

    return musicmap.size() - 1;
}

int reg::font_register(const char *path){
    auto font = std::make_unique<Font>();
    *font = mktxt::LoadFont(path);

    fontmap.push_back(std::move(font));

    return fontmap.size() - 1;
}

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
