#pragma once
#include "mkgl.hpp"
#include "stb_truetype.h"
#include <vector>

typedef stbtt_bakedchar CharPack;

constexpr uint FONT_SIZE_DEFAULT = 64;
constexpr uint FONT_TOTAL_CHARS = 96;

constexpr uint FONT_ATLAS_WIDTH = 512;
constexpr uint FONT_ATLAS_HEIGHT = 512;

struct Font{
    std::vector<byte> data;
    CharPack cpack[96];
    float spacing = 1.0f;
    Texture fontTex;
};

class mktxt{
    static void GenTexture(Texture& tex, const void* data, int width, int height, GLenum format);
public:
    static Font DefaultFont();
    static void UnloadDefaultFont();
    static bool GetFontAtlas(const byte* data, byte* atlas_data, CharPack* pack);

    static Font LoadFont(const char* path);
    static void UnloadFont(Font& font);

    static void RenderTextEx(Font& font, const std::string& text, Vec2 position, float size, Color color);

    static void RenderText(const std::string& text, Vec2 position, float size, Color color);
};
