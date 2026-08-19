#pragma once
#include <cstdint>
#include <vector>
#include "gmath.hpp"
#include "glad.h"

struct Color {
    float r, g, b;
};

constexpr Color White = {1.0f, 1.0f, 1.0f};
constexpr Color Black = {0.0f, 0.0f, 0.0f};
constexpr Color Red = {1.0f, 0.0f, 0.0f};
constexpr Color Green = {0.0f, 1.0f, 0.0f};
constexpr Color Blue = {0.0f, 0.0f, 1.0f};
constexpr Color Yellow = {1.0f, 1.0f, 0.0f};

struct Verts{
    float position[3];
    float normal[3];
    float uv[2];
};

struct Shader {
    uint32_t id;

    GLint uproj;
    GLint uview;
    GLint utex;
};

struct Mesh{
    uint32_t vao, vbo, ebo;
    std::vector<Verts> vertices;
    std::vector<uint32_t> indices;
};

struct Camera2D{
    Vec2 position;
    float rotation;
    float zoom;
    Camera2D(Vec2 position, float rotation, float zoom)
        : position(position), rotation(rotation), zoom(zoom) {}
};

struct Image{
    unsigned char* data;
    int width, height, channels;
};

struct Texture{
    uint32_t id;
    int width, height;
};
