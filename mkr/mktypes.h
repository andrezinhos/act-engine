#pragma once
#include <stdint.h>

#define freeptr(p) do {free(p); p = nullptr;} while(0)

typedef struct Texture {
    uint32_t id;
    int width, height;
} Texture;

typedef enum Cursor{
    NORMAL,
    HIDDEN,
    DISABLED,
} Cursor;

typedef struct{
    uint32_t id;

    uint32_t uview;
    uint32_t umodel;
    uint32_t utex;
} Shader;

typedef struct Rectangle{
    float x, y;
    float width, height;
} Rectangle;

typedef struct {
    uint32_t start;
    uint32_t count;
    Texture* texref;
} DCall;
