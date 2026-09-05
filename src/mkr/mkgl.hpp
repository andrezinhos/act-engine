#pragma once
#include "glad.h"
#include "gmath.hpp"
#include <vector>

typedef unsigned int uint;
typedef unsigned char byte;

constexpr int MKR_POSITION_LAYOUT = 0;
constexpr int MKR_COLOR_LAYOUT = 1;
constexpr int MKR_TEXTURE_LAYOUT = 2;

struct Color {
    float r, g, b;
};

constexpr Color White = {1.0f, 1.0f, 1.0f};
constexpr Color Black = {0.0f, 0.0f, 0.0f};
constexpr Color Red = {1.0f, 0.0f, 0.0f};
constexpr Color Green = {0.0f, 1.0f, 0.0f};
constexpr Color Blue = {0.0f, 0.0f, 1.0f};
constexpr Color Yellow = {1.0f, 1.0f, 0.0f};

enum class types{
    arr,
    buff,
    element,

    prog,
    proj,
    view,
    model,
};

struct vertex{
    float position[3];
    float color[3];
    float uv[2];
};

struct Mesh{
    uint vao, vbo, ebo;
    std::vector<vertex> vertices;
    std::vector<uint> indices;
};

struct Image {
    byte* data;
    int width, height, channels;
};

struct Texture{
    uint id;
    int width, height;
};

struct Shader{
    uint id;

    int uview;
    int umodel;
    int utex;
};

constexpr size_t VMAX = 1000;
constexpr size_t IMAX = 1500;

struct DCall{
    uint start = 0;
    uint count = 0;
    Texture* texref = nullptr;
};

struct Batch{
    std::vector<vertex> vertices;
    std::vector<uint> indices;
    std::vector<DCall> calls;
    uint vao, vbo, ebo;
};

class mkgl{
private:
    static bool getShaderError(uint& shader);
    static bool getShaderProgError(uint& prog);
public:
    static std::vector<byte> loadBytes(const char* path);
    static uint genBuff(types b);
    static void bindBuff(uint& vo, types b);
    static void unbind();
    static void bindDataStatic(types b, const void* data, size_t size);
    static void bindDataDynamic(types b, const void* data, size_t size);
    static void bindSubData(types b, const void* data, size_t size);
    static void sendAttribPtr(int layout, int locSize, int stride, int ptr);

    static Image loadImage(const char* path);
    static void unloadImage(Image& image);
    static uint genTex(GLenum type);
    static void setTexParams(GLenum type, GLenum wrap, GLenum format);
    static void setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data);
    static std::string loadShaderFile(const std::string& path);

    static std::vector<vertex> SetNDC();

    static uint genShader(const char* src, GLenum type);
    static bool compileShader(uint& shader);
    static uint genShaderProg(uint& vs, uint& fs);
    static void setUniformMat(GLint loc, Matrix& mat);
    static void clearScreen(Color color);

    static void Delete(uint& obj, types t);
};
