#pragma once
#include "glad.h"
#include "gmath.hpp"
#include <vector>

typedef unsigned int uint;
typedef unsigned char byte;

static const int MKR_POSITION_LAYOUT = 0;
static const int MKR_COLOR_LAYOUT = 1;
static const int MKR_TEXTURE_LAYOUT = 2;

static const int MKR_VERTEX_STRIDE = 9;

struct Color {
    float r, g, b, a;
};

static const Color White = {1.0f, 1.0f, 1.0f, 1.0f};
static const Color Black = {0.0f, 0.0f, 0.0f, 1.0f};
static const Color Red = {1.0f, 0.0f, 0.0f, 1.0f};
static const Color Green = {0.0f, 1.0f, 0.0f, 1.0f};
static const Color Blue = {0.0f, 0.0f, 1.0f, 1.0f};
static const Color Yellow = {1.0f, 1.0f, 0.0f, 1.0f};

struct vertex{
    float position[3];
    float color[4];
    float uv[2];
};

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

static const size_t VMAX = 1004;
static const size_t IMAX = 1506;

struct DCall{
    uint start = 0;
    uint count = 0;
    Texture* texref = nullptr;
};

typedef struct {
    std::vector<vertex> vertices;
    std::vector<uint> indices;
    std::vector<DCall> calls;
    uint vao, vbo, ebo;
} Batch;

#define freeptr(p) do {free(p); p = nullptr;} while(0)

class mkgl{
private:
    static bool getShaderError(uint* shader);
    static bool getShaderProgError(uint* prog);
    static void getShaderLogInfo(uint* shader, char* log);
    static void getProgramLogInfo(uint* prog, char* log);

    static void linkProgram(uint* prog, uint vs, uint fs);
public:
    static byte* loadBytes(const char* path, size_t* size);
    static void enableBlend(bool flag);
    static void genBuffer(uint* obj);
    static void genArrayBuffer(uint* obj);
    static void bindArrBuff(uint* vo);
    static void bindBuff(uint* vo, GLenum type);
    static void unbind();
    static void bindDataStatic(GLenum type, const void* data, size_t size);
    static void bindDataDynamic(GLenum type, const void* data, size_t size);
    static void bindSubData(GLenum, const void* data, size_t size);
    static void sendAttribPtr(int layout, int locSize, int stride, int ptr);

    static Image loadImage(const char* path);
    static void unloadImage(Image& image);
    static uint genTex(GLenum type);
    static void setTexParams(GLenum type, GLenum wrap, GLenum format);
    static void setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data);
    static char* loadShaderFile(const char* path);

    static std::vector<vertex> SetNDC();

    static uint genShader(const char* src, GLenum type);
    static bool compileShader(uint shader);
    static void genShaderProg(uint* prog, uint vs, uint fs);
    static void deleteShaders(uint vs, uint fs);
    static void setUniformMat(GLint loc, Matrix& mat);
    static void clearScreen(Color color);

    static void deleteVertexArr(uint* obj);
    static void deleteBuffer(uint* obj);
    static void deleteProg(uint* obj);
};
