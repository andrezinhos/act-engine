#pragma once
#include <stdint.h>

#define freeptr(p) do {free(p); p = nullptr;} while(0)

typedef struct Texture {
    uint32_t id;
    int width, height;
} Texture;

typedef struct{
    uint32_t id;

    int uview;
    int umodel;
    int utex;
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
