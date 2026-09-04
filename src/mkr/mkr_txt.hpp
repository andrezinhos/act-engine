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
    Texture fontTex;
};

class mkt{
public:
    static Font DefaultFont();
    static bool GetFontAtlas(const byte* data, byte* atlas_data, CharPack* pack);

    static Font LoadFont(const char* path);
    static void UnloadFont(Font& font);
};
