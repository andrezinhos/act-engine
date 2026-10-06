#pragma once
#include "glad.h"
#include "mkmath.hpp"
#include "mktypes.h"
#include <vector>

#define MKR_POSITION_LAYOUT 0
#define MKR_COLOR_LAYOUT 1
#define MKR_TEXTURE_LAYOUT 2
#define MKR_VERTEX_STRIDE 8

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

namespace mkgl{
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
    void SetVertex();

    uint genShader(cstr src, GLenum type);
    bool compileShader(uint shader);
    void genShaderProg(uint* prog, uint vs, uint fs);
    void deleteShaders(uint vs, uint fs);
    void setUniformMat(GLint loc, Matrix* mat);
    void clearScreen(Color color);

    void deleteVertexArr(uint* obj);
    void deleteBuffer(uint* obj);
    void deleteProg(uint* obj);
};
