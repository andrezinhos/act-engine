#pragma once
#include "glad.h"
#include "mkmath.hpp"
#include <vector>

typedef unsigned int uint;
typedef unsigned char byte;
typedef const char* cstr;

#define MKR_POSITION_LAYOUT 0
#define MKR_COLOR_LAYOUT 1
#define MKR_TEXTURE_LAYOUT 2
#define MKR_VERTEX_STRIDE 9

typedef struct {
    float r;
    float g;
    float b;
    float a;
} Color;

#define White (Color){1.0f, 1.0f, 1.0f, 1.0f}
#define Black (Color){0.0f, 0.0f, 0.0f, 1.0f}
#define Red (Color){1.0f, 0.0f, 0.0f, 1.0f}
#define Green (Color){0.0f, 1.0f, 0.0f, 1.0f}
#define Blue (Color){0.0f, 0.0f, 1.0f, 1.0f}
#define Yellow (Color){1.0f, 1.0f, 0.0f, 1.0f}

typedef struct {
    float position[3];
    float color[4];
    float uv[2];
} vertex;

typedef struct{
    byte* data;
    int width, height, channels;
} Image;

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

#define VMAX 1004
#define IMAX 1506

typedef struct {
    uint start;
    uint count;
    Texture* texref;
} DCall;

typedef struct {
    vertex* vertices;
    int count;
    int cap;
} ArenaV;

typedef struct {
    std::vector<vertex> vertices;
    std::vector<uint> indices;
    std::vector<DCall> calls;
    uint vao, vbo, ebo;
} Batch;

#define freeptr(p) do {free(p); p = nullptr;} while(0)

namespace mkgl{
    bool getShaderError(uint* shader);
    bool getShaderProgError(uint* prog);
    void getShaderLogInfo(uint* shader, char* log);
    void getProgramLogInfo(uint* prog, char* log);
    void linkProgram(uint* prog, uint vs, uint fs);

    byte* loadBytes(cstr path, size_t* size);
    void enableBlend(bool flag);
    void genBuffer(uint* obj);
    void genArrayBuffer(uint* obj);
    void bindArrBuff(uint* vo);
    void bindBuff(uint* vo, GLenum type);
    void unbind();
    void bindDataStatic(GLenum type, const void* data, size_t size);
    void bindDataDynamic(GLenum type, const void* data, size_t size);
    void bindSubData(GLenum, const void* data, size_t size);
    void sendAttribPtr(int layout, int locSize, int stride, int ptr);

    char* loadShaderFile(cstr path);
    std::vector<vertex> SetNDC();
    vertex* SetVertex();

    uint genShader(cstr src, GLenum type);
    bool compileShader(uint shader);
    void genShaderProg(uint* prog, uint vs, uint fs);
    void deleteShaders(uint vs, uint fs);
    void setUniformMat(GLint loc, Matrix& mat);
    void clearScreen(Color color);

    void deleteVertexArr(uint* obj);
    void deleteBuffer(uint* obj);
    void deleteProg(uint* obj);
};
