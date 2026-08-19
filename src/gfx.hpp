#pragma once
#include "gmath.hpp"
#include "utils.hpp"

namespace gfx{
    Shader LoadShader(const char* vs, const char* fs);
    void UnloadShader(Shader& shader);

    Image LoadImage(const char* path);
    Texture LoadTexture(Image& image);

    void UnloadImage(Image& image);
    void UnloadTexture(Texture& texture);

    void CreateQuad(Mesh& mesh, Color color);
    void DrawMesh(Texture* tex = nullptr);
    void DrawTexture(Texture* tex, Vec2 position, Vec2 size, Color color);

    void UnloadMesh(Mesh& mesh);
};
