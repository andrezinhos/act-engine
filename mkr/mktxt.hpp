#pragma once
#include "tgt.h"
#include "mkmath.hpp"
#include "mktypes.h"
#include "tfb.h"

#define FONT_SIZE_DEFAULT 64
#define FONT_TOTAL_CHARS 96

#define FONT_ATLAS_WIDTH 512
#define FONT_ATLAS_HEIGHT 512

typedef struct Font{
    uint8_t* data;
    glyph pack[96];
    tfb_base base;
    float spacing;
    Texture fontTex;
} Font;

namespace mktxt{
    Font default_font();
    void unload_default_font();

    Font load_font(const char* path);
    void unload_font(Font* font);

    void render_text_extended(Font* font, const char* text, Vec2 position, float size, color color);
    void render_text(const char* text, Vec2 position, float size, color color);
};
