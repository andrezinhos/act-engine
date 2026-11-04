// #define TFB_IMPL
#include "mkr.h"
#include "tfb.h"
#include "bml.h"

bool mk_load_default_font(){
    dfont.spacing = 1.0f;

    if (!tfb_load("eng/Tiny5.ttf", &dfont.data, &dfont.base.info)){
        printf("error: failed to find default font\n");
        return false;
    }

    if (!tfb_set_atlas(&dfont.base, 512, 512)){
        printf("error: failed to create default font atlas\n");
        return false;
    }

    tfb_font_metrics(&dfont.base);

    int pen_x = 0;
    int pen_y = 0;
    int row_h = 0;

    for (int i = 0; i < 96; i++){
        int codep = i + 32;
        int gw, gh;

        tfb_glyph_metrics(&dfont.base, dfont.pack, i, codep, &gw, &gh);

        if (gw <= 0 || gh <= 0) {
            continue;
        }

        //jump next line
        if (pen_x + gw + 1 > dfont.base.atlas.width){
            pen_x = 0;
            pen_y += row_h + 1;
            row_h = 0;
        }

        tfb_make_glyph(&dfont.base, codep, pen_x, pen_y, gw, gh);
        tfb_glyph_set(dfont.pack, i, gw, gh, pen_x, pen_y);

        // prepare next glyph
        pen_x += gw + 1;
        if (gh > row_h) row_h = gh;
    }

    // stbi_write_png("baking.png", font.base.atlas.width, font.base.atlas.height, 1, font.base.atlas.data, font.base.atlas.width);

    uint8_t* rgba = (uint8_t*)malloc(512 * 512 * 4);
    for(int i = 0; i < 512*512; i++){
        rgba[i*4 + 0] = 255;
        rgba[i*4 + 1] = 255;
        rgba[i*4 + 2] = 255;
        rgba[i*4 + 3] = dfont.base.atlas.data[i];
    }
    dfont.ftex = mk_load_tex_memory(rgba, 512, 512);

    free(dfont.data);
    free(dfont.base.atlas.data);
    free(rgba);

    return true;
}

void mk_unload_default_font(){
    fgl_delete_texture(&dfont.ftex.id);
}

void mk_render_text(font* font, const char* text, vec2 position, float min_sz){
    float scale = min_sz / 64.0f;
    float lineHeight = 64.0f * scale * font->spacing;
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

        rectangle src = {
            (float)ch->x0,
            (float)ch->y0,
            (float)(ch->x1 - ch->x0),
            (float)(ch->y1 - ch->y0)
        };

        vec2 pos = {
            position.x + (ch->xoff * scale),
            position.y + (ch->yoff * scale)
        };

        vec2 size = {
            src.width * scale,
            src.height * scale
        };

        mk_render_tex_region(
            &font->ftex,
            src,
            pos,
            size,
            White
        );

        position.x += ch->xadvance * scale;
    }
}
