#pragma once
#include "mktypes.h"
#include "mkmath.hpp"
#include "stb_truetype.h"

typedef stbtt_bakedchar CharPack;

#define FONT_SIZE_DEFAULT 64
#define FONT_TOTAL_CHARS 96

#define FONT_ATLAS_WIDTH 512
#define FONT_ATLAS_HEIGHT 512

typedef struct{
    byte* data;
    CharPack cpack[96];
    float spacing;
    Texture fontTex;
} Font;

namespace mktxt{
    Font DefaultFont();
    void UnloadDefaultFont();

    Font LoadFont(const char* path);
    void UnloadFont(Font* font);

    void RenderTextEx(Font* font, const char* text, Vec2 position, float size, Color color);
    void RenderText(const char* text, Vec2 position, float size, Color color);
};
