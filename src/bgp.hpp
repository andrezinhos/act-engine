#pragma once
#include "glad.h"
#include <cstdint>
#include <vector>
#include <string>
#include "utils.hpp"

enum type {
    ARR,
    BUF,
    ELM,

    PROJ,
    VIEW,
    TEX_2D,
    PROG,
    SHAD,

    STATIC,
    DYNAMIC,
};

struct DCall{
    uint32_t start = 0;
    uint32_t count = 0;
    Texture* texref = nullptr;
};

struct Batch{
    std::vector<Verts> verts;
    std::vector<uint32_t> indices;
    std::vector<DCall> calls;
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
};

namespace bgp{
    inline Batch batch;
    std::vector<uint8_t> LoadBytes(const char* path);
    std::string LoadShaderFile(const char* path);
    bool ShaderError(uint32_t& src);

    std::vector<Verts> SetVertsVal(Vec2 position, Vec2 size, Color color);
    std::vector<Verts> SetVertsVal(Vec3 position, Vec2 size, Color color);
    std::vector<Verts> SetStdNDC();
    void SendIndices(uint32_t base);

    void Flush();
    void FigureDraw(size_t size, void* offset);

    Matrix GetViewMatrix(Camera2D& cam);
    Matrix GetProjectionMatrix(int width, int height);

    void SetUniform(GLint type, Matrix& mat);
    void GetUniformLoc(Shader& shader, type t, const char* name);

    void CreateObject(uint32_t& vo, type t);
    void BindObject(uint32_t& vo, type t);
    void BindObject(int vo, type t);
    void BindData(type t, size_t size, const void* data, type draw);
    void BindSubData(type t, size_t size, const void* data);
    void AttribPtr(int loc, int locSize, int size, int ptr);

    void TexParams(GLenum form, GLenum texstyle, GLenum map);
    void GenTexture(uint32_t& id);
    void SetTex2D(GLenum color, int width, int height, const void* data);

    Shader DefaultShader();
    Texture DefaultTexture();
    Mesh DefaultQuad();
    void DefaultBatch();

    void Delete(type t, uint32_t& obj);
};
