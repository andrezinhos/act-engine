#pragma once
#include <stdint.h>

typedef struct vertex{
    float position[3];
    float color[3];
    float uv[2];
} vertex;

typedef struct color{
    float r;
    float g;
    float b;
} color;

// typedef enum{
//     NORMAL,
//     HIDDEN,
//     DISABLED
// } cursor;

typedef struct texture {
    uint32_t id;
    int width;
    int height;
} texture;

typedef struct rectangle{
    float x;
    float y;
    float width;
    float height;
} rectangle;

typedef struct shader{
    uint32_t id;

    uint32_t utex;
    uint32_t umodel;
    uint32_t uproj;
    uint32_t uview;
} shader;

#define White (color){1.0f, 1.0f, 1.0f}
#define Black (color){0.0f, 0.0f, 0.0f}
#define Red (color){1.0f, 0.0f, 0.0f}
#define Green (color){0.0f, 1.0f, 0.0f}
#define Blue (color){0.0f, 0.0f, 1.0f}
#define Yellow (color){1.0f, 1.0f, 0.0f}
#define Cyan (color){1.0f, 0.0f, 1.0f}
