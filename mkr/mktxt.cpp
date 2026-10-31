#include "fgl.h"
#define STB_TRUETYPE_IMPLEMENTATION
#define TFB_IMPL

#include "mktxt.hpp"
#include "mktex.hpp"
#include "mkr.hpp"
#include <stddef.h>
#include <stdio.h>

Texture GenTexture(void* data, int width, int height){
    Texture tex = {};
    tex.width = width;
    tex.height = height;

    mktex::create_tex(&tex.id, GL_TEXTURE_2D, width, height, data);
    fgl_texture_repeat(tex.id);
    fgl_texture_filter(tex.id, GL_NEAREST);

    return tex;
}

Font mktxt::default_font(){
    Font font = {};

    font.spacing = 1.0f;

    if (tfb_load("eng/Tiny5.ttf", &font.data, &font.base.info) != 0){
        return font;
    }

    tfb_set_atlas(&font.base, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT);
    tfb_font_metrics(&font.base);

    int pen_x = 0;
    int pen_y = 0;
    int row_h = 0;

    for (int i = 0; i < 96; i++){
        int codep = i + 32;
        int gw, gh;

        tfb_glyph_metrics(&font.base, font.pack, i, codep, &gw, &gh);

        if (gw <= 0 || gh <= 0) {
            continue;
        }

        if (pen_x + gw + 1 > font.base.atlas.width){
            pen_x = 0;
            pen_y += row_h + 1;
            row_h = 0;
        }

        tfb_make_glyph(&font.base, codep, pen_x, pen_y, gw, gh);
        tfb_glyph_set(font.pack, i, gw, gh, pen_x, pen_y);

        // prepare next glyph
        pen_x += gw + 1;
        if (gh > row_h) row_h = gh;
    }

    // stbi_write_png("baking.png", font.base.atlas.width, font.base.atlas.height, 1, font.base.atlas.data, font.base.atlas.width);

    uint8_t* rgba = (uint8_t*)malloc(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT * 4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255;
        rgba[i*4 + 1] = 255;
        rgba[i*4 + 2] = 255;
        rgba[i*4 + 3] = font.base.atlas.data[i];
    }
    font.fontTex = GenTexture(rgba, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT);

    // mktex::tex_valid(&font.fontTex.id);

    free(font.data);
    free(font.base.atlas.data);
    free(rgba);
    return font;
};

void mktxt::unload_default_font(){
    if(dstate->dfont.fontTex.id != 0) {
        glDeleteTextures(1, &dstate->dfont.fontTex.id);
    }
}

Font mktxt::load_font(const char* path){
    Font font = {};

    font.spacing = 1.0f;

    if (tfb_load(path, &font.data, &font.base.info) != 0){
        return font;
    }

    tfb_set_atlas(&font.base, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT);
    tfb_font_metrics(&font.base);

    int pen_x = 0;
    int pen_y = 0;
    int row_h = 0;

    for (int i = 0; i < 96; i++){
        int codep = i + 32;
        int gw, gh;

        tfb_glyph_metrics(&font.base, font.pack, i, codep, &gw, &gh);

        if (gw <= 0 || gh <= 0) {
            continue;
        }

        if (pen_x + gw + 1 > font.base.atlas.width){
            pen_x = 0;
            pen_y += row_h + 1;
            row_h = 0;
        }

        tfb_make_glyph(&font.base, codep, pen_x, pen_y, gw, gh);
        tfb_glyph_set(font.pack, i, gw, gh, pen_x, pen_y);

        // prepare next glyph
        pen_x += gw + 1;
        if (gh > row_h) row_h = gh;
    }

    // stbi_write_png("baking.png", font.base.atlas.width, font.base.atlas.height, 1, font.base.atlas.data, font.base.atlas.width);

    uint8_t* rgba = (uint8_t*)malloc(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT * 4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255;
        rgba[i*4 + 1] = 255;
        rgba[i*4 + 2] = 255;
        rgba[i*4 + 3] = font.base.atlas.data[i];
    }
    font.fontTex = GenTexture(rgba, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT);

    // mktex::tex_valid(&font.fontTex.id);

    free(font.data);
    free(font.base.atlas.data);
    free(rgba);
    return font;
};

void mktxt::unload_font(Font* font){
    if (font->fontTex.id != 0) {
        glDeleteTextures(1, &font->fontTex.id);
        printf("[INFO] FONT UNLOADED\n");
    }
}

void mktxt::render_text_extended(Font *font, const char* text, Vec2 position, float minSize, color color){
    float scale = minSize / FONT_SIZE_DEFAULT;
    float lineHeight = FONT_SIZE_DEFAULT * scale * font->spacing;
    float startX = position.x;

    for(int i = 0; text[i] != '\0'; i++){
        char c = text[i];
        if (c == '\n'){
            position.x = startX;
            position.y += lineHeight;
            continue;
        }

        if (c == '\r') {
            continue;
        }

        if (c < 32 || c >= 127) {
            continue;
        }
        glyph* ch = &font->pack[c - 32];

        Rectangle src = {
            (float)ch->x0,
            (float)ch->y0,
            (float)(ch->x1 - ch->x0),
            (float)(ch->y1 - ch->y0)
        };

        Vec2 pos = {
            position.x + (ch->xoff * scale),
            position.y + (ch->yoff * scale)
        };

        Vec2 size = {
            src.width * scale,
            src.height * scale
        };

        mktex::render_tex_rect(
            &font->fontTex,
            src,
            pos,
            size,
            color
        );

        position.x += ch->xadvance * scale;
    }
}

void mktxt::render_text(const char* text, Vec2 position, float minSize, color color){
    render_text_extended(&dstate->dfont, text, position, minSize, color);
}
