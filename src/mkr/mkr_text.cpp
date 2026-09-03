#define STB_TRUETYPE_IMPLEMENTATION
#include "mkr.hpp"

void InitFont(byte* data, FontInfo& info){
    bool result = stbtt_InitFont(&info, data, 0);
    if (!result){
        printf("[INFO] FONT CANNOT BE LOADED");
    }
}

Font mkr::DefaultFont(){
    Font font = {};

    font.buff = mkgl::loadBytes("assets/fonts/Tiny5-Regular.ttf");

    InitFont(font.buff.data(), font.info);

    std::vector<byte> atlas(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, 0);
 
    FontPack pack;
    stbtt_PackBegin(&pack, atlas.data(), FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, 0, 1, nullptr);

    stbtt_PackFontRange(&pack, font.buff.data(), 0, FONT_SIZE_DEFAULT, 32, FONT_TOTAL_CHARS, font.cpack);
    stbtt_PackEnd(&pack);

    std::vector<unsigned char> rgba(512*512*4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255; // R
        rgba[i*4 + 1] = 255; // G
        rgba[i*4 + 2] = 255; // B
        rgba[i*4 + 3] = atlas[i]; // A = dado da fonte
    }

    GenTexture(font.fontTex, rgba.data(), FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, GL_RGBA);
    return font;
};

void mkr::RenderText(const std::string& text, Vec2 position, Vec2 size, Color color){
    for(char c : text){
        if (c < 32) continue;
        stbtt_packedchar* ch = &mkgl::state.dfont.cpack[c - 32];

        Rectangle src = {
            (float)ch->x0,
            (float)ch->y0,
            (float)(ch->x1 - ch->x0),
            (float)(ch->y1 - ch->y0)
        };
        
        Vec2 pos  = { position.x + ch->xoff, position.y + ch->yoff };
        Vec2 size = { (float)src.width, (float)src.height };
        
        RenderTextureRec(
            &mkgl::state.dfont.fontTex, 
            src,
            pos, 
            size, 
            color
        );
        
        position.x += ch->xadvance;
    }
}
