#include "gfx.hpp"
#include "core.hpp"
#include "utils.hpp"
#include <cstdint>
#include <cstdio>
#include "bgp.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Shader gfx::LoadShader(const char *vs, const char *fs){
    Shader shader = {};
    std::string vsPath = bgp::LoadShaderFile(vs);
    std::string fsPath = bgp::LoadShaderFile(fs);

    const char* srcvs = vsPath.c_str();
    const char* srcfs = fsPath.c_str();

    uint32_t vsFile = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsFile, 1, &srcvs, nullptr);
    glCompileShader(vsFile);

    uint32_t fsFile = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsFile, 1, &srcfs, nullptr);
    glCompileShader(fsFile);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vsFile);
    glAttachShader(shader.id, fsFile);
    glLinkProgram(shader.id);

    int pass;
    char log[512];
    glGetProgramiv(shader.id, GL_LINK_STATUS, &pass);
    if (!pass){
        glGetProgramInfoLog(shader.id, 512, nullptr, log);
        printf("Error to compile program:\n%s", log);
    } else printf("Shader compiled successfully\n");

    glDeleteShader(vsFile);
    glDeleteShader(fsFile);

    return shader;
}

void gfx::UnloadShader(Shader &shader){
    if (shader.id != 0){
        glDeleteProgram(shader.id);
        shader.id = 0;
        printf("Shader Unloaded\n");
    }
}

Image gfx::LoadImage(const char *path){
    Image image = {};

    std::vector<uint8_t> bytes = bgp::LoadBytes(path);

    image.data = stbi_load_from_memory(
        bytes.data(),
        bytes.size(),
        &image.width,
        &image.height,
        &image.channels,
        4
    );

    return image;
}

Texture gfx::LoadTexture(Image &image){
    Texture tex = {};

    bgp::GenTexture(tex.id);
    bgp::SetTex2D(GL_RGBA, image.width, image.height, image.data);
    UnloadImage(image);

    return tex;
}

void gfx::UnloadImage(Image &image){
    stbi_image_free(image.data);
}

void gfx::UnloadTexture(Texture &tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("Texture Unloaded\n");
}

void gfx::DrawTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    uint32_t base = bgp::batch.verts.size();
    uint32_t indexStart = bgp::batch.indices.size();

    auto verts = bgp::SetVertsVal(position, size, color);
    bgp::SendIndices(base);

    for (const auto& v : verts) bgp::batch.verts.push_back(v);
    bgp::SendIndices(base);

    if (bgp::batch.calls.empty() || bgp::batch.calls.back().texref != tex){
        bgp::batch.calls.push_back({indexStart, 6, tex});
    }
}

void gfx::CreateQuad(Mesh &mesh, Color color){
    mesh.vertices = {
        { {0.5f,   0.5f, 0.0f}, {color.r, color.g, color.b}, {0.0f, 0.0f} },
        { {0.5f,  -0.5f, 0.0f}, {color.r, color.g, color.b}, {1.0f, 0.0f} },
        { {-0.5f, -0.5f, 0.0f}, {color.r, color.g, color.b}, {1.0f, 1.0f} },
        { {-0.5f,  0.5f, 0.0f}, {color.r, color.g, color.b}, {0.0f, 1.0f} }
    };

    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    bgp::CreateObject(mesh.vao, ARR);
    bgp::CreateObject(mesh.vbo, BUF);
    bgp::CreateObject(mesh.ebo, BUF);

    bgp::BindObject(mesh.vao, ARR);

    bgp::BindObject(mesh.vbo, BUF);
    bgp::BindData(
        BUF,
        mesh.vertices.size() * sizeof(Verts),
        mesh.vertices.data(), STATIC
    );

    bgp::BindObject(mesh.ebo, ELM);
    bgp::BindData(
        ELM,
        mesh.indices.size() * sizeof(uint32_t),
        mesh.indices.data(), STATIC
    );

    bgp::AttribPtr(0, 3, 8, 0);
    bgp::AttribPtr(1, 3, 8, 3);
    bgp::AttribPtr(2, 2, 8, 6);
}

void gfx::DrawMesh(Texture* tex){
    glActiveTexture(GL_TEXTURE0);

    if (tex != nullptr) glBindTexture(GL_TEXTURE_2D, tex->id);
    else glBindTexture(GL_TEXTURE_2D, core::state.dtex.id);

    bgp::BindObject(core::state.dmesh.vao, ARR);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void gfx::UnloadMesh(Mesh &mesh){
    bgp::Delete(ARR, mesh.vao);
    bgp::Delete(BUF, mesh.vbo);
    bgp::Delete(BUF, mesh.ebo);
    printf("Mesh Unloaded\n");
}
