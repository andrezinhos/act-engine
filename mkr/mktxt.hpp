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
    Font DefaultFont();
    void UnloadDefaultFont();

    Font load_font(const char* path);
    void UnloadFont(Font* font);

    void RenderTextEx(Font* font, const char* text, Vec2 position, float size, color color);
    void RenderText(const char* text, Vec2 position, float size, color color);
};
