#define STB_TRUETYPE_IMPLEMENTATION
#include "mkr.hpp"

void mkr::GenTexture(Texture& tex, const void* data, int width, int height, GLenum format){
    tex.id = mkgl::genTex(GL_TEXTURE_2D);
    tex.width = width;
    tex.height = height;

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mkgl::setTexImage2D(GL_RGBA, format, width, height, data);
}

bool mkt::GetFontAtlas(const byte* data, byte* atlas_data, CharPack* pack){
    bool result = stbtt_BakeFontBitmap(data, 0, 32.0f, atlas_data, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, 32, 96, pack);
    if (!result){
        printf("[INFO] ERROR TO SETUP FONT (BITMAP)");
        return false;
    }
    return true;
}

Font mkr::DefaultFont(){
    Font font = {};

    font.data = mkgl::loadBytes("assets/fonts/Tiny5-Regular.ttf");

    std::vector<byte> atlas(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, 0);
    
    mkt::GetFontAtlas(font.data.data(), atlas.data(), font.cpack);

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

void mkt::UnloadFont(Font& font){
    if (font.fontTex.id != 0) glDeleteTextures(1, &font.fontTex.id);
    printf("[INFO] FONT UNLOADED");
}

void mkr::RenderText(const std::string& text, Vec2 position, float minSize, Color color){
    float scale = minSize / 32.0f;

    for(char c : text){
        if (c < 32 || c >= 127) continue;
        stbtt_bakedchar* ch = &mkr::state.dfont.cpack[c - 32];

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
        
        RenderTextureRec(
            &mkr::state.dfont.fontTex, 
            src,
            pos, 
            size, 
            color
        );
        
        position.x += ch->xadvance * scale;
    }
}
