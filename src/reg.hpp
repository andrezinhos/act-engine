#pragma once
#include "mktypes.h"
#include "mktxt.hpp"
#include "pwra.h"
#include <vector>
#include <memory>

extern std::vector<std::unique_ptr<Texture>> texmap;
extern std::vector<std::unique_ptr<Sfx>> soundmap;
extern std::vector<std::unique_ptr<Stream>> musicmap;
extern std::vector<std::unique_ptr<Font>> fontmap;


namespace reg{
    void init();

    int tex_register(const char* path);
    int sfx_register(const char* path);
    int stream_register(const char* path);
    int font_register(const char* path);

    void clear_tex();
    void clear_sfx();
    void clear_stream();
    void clear_font();

    void clear();
};
