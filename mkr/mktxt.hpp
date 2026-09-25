#pragma once
#include "mkgl.hpp"
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

class mktxt{
    static void GenTexture(Texture& tex, const void* data, int width, int height, GLenum format);
    static bool GetFontAtlas(const byte* data, byte* atlas_data, CharPack* pack);
public:
    static Font DefaultFont();
    static void UnloadDefaultFont();

    static Font LoadFont(const char* path);
    static void UnloadFont(const Font& font);

    static void RenderTextEx(Font& font, const char* text, Vec2 position, float size, Color color);
    static void RenderText(const char* text, Vec2 position, float size, Color color);
};
