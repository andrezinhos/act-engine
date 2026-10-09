#pragma once
#include <string_view>

struct Sfx;
struct Stream;
struct Texture;
struct Font;

namespace reg{
    void init();

    int tex_register(std::string_view path);
    int sfx_register(std::string_view path);
    int stream_register(std::string_view path);
    int font_register(std::string_view path);

    Texture* getTex(int id);
    Sfx* getSfx(int id);
    Stream* getMusic(int id);
    Font* getFont(int id);

    void clear_tex();
    void clear_sfx();
    void clear_stream();
    void clear_font();

    void clear();
};
