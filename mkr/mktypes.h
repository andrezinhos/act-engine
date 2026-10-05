#pragma once

typedef unsigned int uint;
typedef unsigned char byte;
typedef const char* cstr;

#define freeptr(p) do {free(p); p = nullptr;} while(0)

typedef struct {
    float r;
    float g;
    float b;
} Color;

#define White (Color){1.0f, 1.0f, 1.0f}
#define Black (Color){0.0f, 0.0f, 0.0f}
#define Red (Color){1.0f, 0.0f, 0.0f}
#define Green (Color){0.0f, 1.0f, 0.0f}
#define Blue (Color){0.0f, 0.0f, 1.0f}
#define Yellow (Color){1.0f, 1.0f, 0.0f}

typedef enum{
    NORMAL,
    HIDDEN,
    DISABLED
} Cursor;

typedef struct {
    float position[3];
    float color[3];
    float uv[2];
} vertex;

typedef struct {
    uint id;
    int width, height;
} Texture;

typedef struct{
    uint id;

    int uview;
    int umodel;
    int utex;
} Shader;

struct Rectangle{
    float x, y;
    float width, height;
};
