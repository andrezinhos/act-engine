#include "mkr.hpp"
#include "mktxt.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

void mktxt::GenTexture(Texture& tex, const void* data, int width, int height, GLenum format){
    tex.id = mkgl::genTex(GL_TEXTURE_2D);
    tex.width = width;
    tex.height = height;

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mkgl::setTexImage2D(GL_RGBA, format, width, height, data);
}

bool mktxt::GetFontAtlas(const byte* data, byte* atlas_data, CharPack* pack){
    bool result = stbtt_BakeFontBitmap(
        data, 0, 
        FONT_SIZE_DEFAULT, 
        atlas_data, 
        FONT_ATLAS_WIDTH, 
        FONT_ATLAS_HEIGHT, 
        32, 
        FONT_TOTAL_CHARS, 
        pack
    );

    if (!result){
        printf("[INFO] ERROR TO SETUP FONT (BITMAP)");
        return false;
    }
    return true;
}

Font mktxt::DefaultFont(){
    Font font = {};

    font.data = mkgl::loadBytes("assets/fonts/Tiny5.ttf");

    std::vector<byte> atlas(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, 0);
    
    mktxt::GetFontAtlas(font.data.data(), atlas.data(), font.cpack);

    std::vector<byte> rgba(512*512*4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255; 
        rgba[i*4 + 1] = 255; 
        rgba[i*4 + 2] = 255;
        rgba[i*4 + 3] = atlas[i];
    }
    GenTexture(font.fontTex, rgba.data(), FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, GL_RGBA);

    font.data.clear();
    atlas.clear();
    return font;
};

Font mktxt::LoadFont(const char* path){
    Font font = {};

    font.data = mkgl::loadBytes(path);

    std::vector<byte> atlas(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, 0);
    
    mktxt::GetFontAtlas(font.data.data(), atlas.data(), font.cpack);

    std::vector<byte> rgba(512*512*4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255; 
        rgba[i*4 + 1] = 255; 
        rgba[i*4 + 2] = 255;
        rgba[i*4 + 3] = atlas[i];
    }
    GenTexture(font.fontTex, rgba.data(), FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, GL_RGBA);

    font.data.clear();
    atlas.clear();
    return font;
};

void mktxt::UnloadDefaultFont(){
    if(mkr::state.dfont.fontTex.id != 0) glDeleteTextures(1, &mkr::state.dfont.fontTex.id);
}

void mktxt::UnloadFont(Font& font){
    if (font.fontTex.id != 0) glDeleteTextures(1, &font.fontTex.id);
    printf("[INFO] FONT UNLOADED\n");
}

void mktxt::RenderTextEx(Font &font, const std::string &text, Vec2 position, float minSize, Color color){
    float scale = minSize / FONT_SIZE_DEFAULT;
    
    float lineHeight = FONT_SIZE_DEFAULT * scale * font.spacing;
    float startX = position.x;
    for(char c : text){
        if (c == '\n'){
            position.x = startX;
            position.y += lineHeight;
            continue;
        }

        if (c == '\r') continue;

        if (c < 32 || c >= 127) continue;
        stbtt_bakedchar* ch = &font.cpack[c - 32];

        Rectangle src = {
            (float)ch->x0,
            (float)ch->y0,
            (float)(ch->x1 - ch->x0),
            (float)(ch->y1 - ch->y0)
        };
        
        Vec2 pos  = {
            position.x + (ch->xoff * scale), 
            position.y + (ch->yoff * scale)
        };
        
        Vec2 size = { 
            src.width * scale, 
            src.height * scale 
        };
        
        mkr::RenderTextureRec(
            &font.fontTex, 
            src,
            pos, 
            size, 
            color
        );
        
        position.x += ch->xadvance * scale;
    }
}

void mktxt::RenderText(const std::string& text, Vec2 position, float minSize, Color color){
    RenderTextEx(mkr::state.dfont, text, position, minSize, color);
}
