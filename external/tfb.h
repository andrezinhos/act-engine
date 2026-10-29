#pragma once
#include "stb_truetype.h"
#include <cstdint>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct glyph{
    float xoff, yoff;
    float xadvance;
    uint16_t x0, x1, y0, y1;
} glyph;

typedef struct atlas{
    int width, height;
    uint8_t* data;
} atlas;

typedef struct tfb_base{
    stbtt_fontinfo info;
    atlas atlas;
    int ascent;
    int descent;
    int lineGap;
    float scale;
} tfb_base;

#ifdef TFB_STATIC
    #define TFBDEF static inline
#else
    #define TFBDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

TFBDEF int tfb_load(const char* path, uint8_t** data, stbtt_fontinfo* info);
TFBDEF int tfb_set_atlas(tfb_base* atlas, int w, int h);
TFBDEF void tfb_font_metrics(tfb_base* base);
TFBDEF void tfb_glyph_metrics(tfb_base* base, glyph* g, int index, int codep, int* gw, int* gh);
TFBDEF void tfb_make_glyph(tfb_base* base, int codep, int pen_x, int pen_y, int gw, int gh);
TFBDEF void tfb_glyph_set(glyph* g, int index, int gw, int gh, int pen_x, int pen_y);

#ifdef TFB_IMPL

static inline uint8_t* load_file(const char* path, size_t* size){
    FILE* file = fopen(path, "rb");

    if (!file) {
        printf("error: file failed to load or doesn't exist\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    uint8_t* buffer = (uint8_t*)malloc(fsize);

    if (!buffer){
        printf("error: coudn't create file buffer\n");
        return NULL;
    }

    size_t read = fread(buffer, 1, fsize, file);
    fclose(file);

    if (read != fsize){
        printf("error: failed to read file\n");
        free(buffer);
        return NULL;
    }

    *size = fsize;
    return buffer;
}

TFBDEF int tfb_load(const char* path, uint8_t** data, stbtt_fontinfo* info){
    size_t fsize = 0;
    *data = load_file(path, &fsize);

    if (!*data){
        return 1;
    }

    stbtt_InitFont(info, *data, 0);

    return 0;
}

TFBDEF int tfb_set_atlas(tfb_base* base, int w, int h){
    base->atlas.data = (uint8_t*)calloc(w * h, 1);

    if (!base->atlas.data){
        printf("error: failed to create font atlas\n");
        return 1;
    }

    base->atlas.width = w;
    base->atlas.height = h;

    return 0;
}

TFBDEF void tfb_font_metrics(tfb_base* base){
    base->scale = stbtt_ScaleForPixelHeight(&base->info, 64.0f);

    stbtt_GetFontVMetrics(
        &base->info,
        &base->ascent,
        &base->descent,
        &base->lineGap
    );
}

TFBDEF void tfb_glyph_metrics(tfb_base* base, glyph* g, int index, int codep, int* gw, int* gh){
    int advance, lsb;
    int bx0, by0, bx1, by1;

    stbtt_GetCodepointHMetrics(&base->info, codep, &advance, &lsb);
    stbtt_GetCodepointBitmapBox(&base->info, codep, base->scale, base->scale, &bx0, &by0, &bx1, &by1);

    *gw = bx1 - bx0;
    *gh = by1 - by0;

    g[index].xoff = (float)bx0;
    g[index].yoff = (float)by0;
    g[index].xadvance = advance * base->scale;
}

TFBDEF void tfb_make_glyph(tfb_base* base, int codep, int pen_x, int pen_y, int gw, int gh){
    stbtt_MakeCodepointBitmap(&base->info,
        base->atlas.data + pen_x + pen_y * base->atlas.width,
        gw, gh,
        base->atlas.width,
        base->scale, base->scale,
        codep
    );
}

TFBDEF void tfb_glyph_set(glyph* g, int index, int gw, int gh, int pen_x, int pen_y){
    g[index].x0 = pen_x;
    g[index].y0 = pen_y;
    g[index].x1 = pen_x + gw;
    g[index].y1 = pen_y + gh;
}

#endif

#ifdef __cplusplus
}
#endif
