#pragma once
#include "mktypes.h"
#include "mkmath.hpp"
#include "tgt.h"

namespace mktex{
    Texture DefaultTexture();
    void UnloadDefaultTexture();

    Texture load_tex_src(void* data, int width, int height);
    void unload_tex(Texture* tex);

    void render_tex_rect(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, color color);
    void render_tex(Texture* tex, Vec2 position, Vec2 size, color color);

    int tex_valid(uint32_t* tex);

    int create_tex(uint32_t* id, int format, int width, int height, void* data);
};
