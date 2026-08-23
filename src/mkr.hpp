#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "gmath.hpp"
#include "utils.hpp"

enum type {
    ARR,
    BUF,
    ELM,

    PROJ,
    VIEW,
    MODEL,
    TEX_2D,
    PROG,
    SHAD,

    STATIC,
    DYNAMIC,

    NEAREST,
    LINEAR,
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

class mkr{
private:
    static std::vector<unsigned char> LoadBytes(const char* path);
    static std::string LoadShaderFile(const char* path);
    static bool ShaderError(unsigned char& src);

    static void SetVertsVal(Vec2 position, Vec2 size, Color color);
    static void SetVertsVal(Vec3 position, Vec2 size, Color color);
    static std::vector<Verts> SetStdNDC();
    static void SendIndices(uint32_t base);

    static void TextureActive(int num);

    static void Flush();
    static void FigureDraw(type t, size_t size, void* offset);

    static void SetUniform(GLint type, Matrix& mat);
    static void GetUniformLoc(Shader& shader, type t, const char* name);

    static void GenObject(uint32_t& vo, type t);
    static void BindObject(uint32_t& vo, type t);
    static void BindObject(int vo, type t);
    static void BindData(type t, size_t size, const void* data, type draw);
    static void BindSubData(type t, size_t size, const void* data);
    static void AttribPtr(int loc, int locSize, int size, int ptr);

    static void TexParams(GLenum form, GLenum texstyle, GLenum map);
    static void SetTex2D(GLenum color, int width, int height, const void* data);

    static void TemplateBuf(uint32_t& vao, uint32_t& vbo, const void* data, size_t size, type draw);
    static void TemplateBuf(uint32_t& vao, uint32_t& vbo, uint32_t& ebo, const void* datav,
        const void* datai, size_t sizev, size_t sizei, type draw);

    static Shader DefaultShader();
    static Texture DefaultTexture();
    static Mesh DefaultQuad();
    static void DefaultBatch();
    static void Delete(type t, uint32_t& obj);
    static void Delete(type t, unsigned char& obj);

    friend class core;
public:
    static Batch batch;
    static Shader LoadShader(const char* vs, const char* fs);
    static void UnloadShader(Shader& shader);

    static Image LoadImage(const char* path);
    static Texture LoadTexture(Image& image, type t);

    static void UnloadImage(Image& image);
    static void UnloadTexture(Texture& texture);

    static void DrawRectangle(Vec2 position, float size, Color color);
    static void DrawTexture(Texture* tex, Vec2 position, Vec2 size, Color color);
    static void DrawCube(Vec3 pos, Vec3 size, Color color);

    static void UnloadMesh(Mesh& mesh);
};
