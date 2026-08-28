#include "mkr.hpp"
#include "core.hpp"
#include "utils.hpp"
#include <cstdint>
#include <cstdio>
#include "stb_image.h"

/* RENDER IMPLEMENTATION */

Shader mkr::LoadShader(const char *vs, const char *fs){
    Shader shader = {};
    std::string vsPath = mkr::LoadShaderFile(vs);
    std::string fsPath = mkr::LoadShaderFile(fs);

    const char* srcvs = vsPath.c_str();
    const char* srcfs = fsPath.c_str();

    unsigned int vsFile = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsFile, 1, &srcvs, nullptr);
    glCompileShader(vsFile);

    unsigned int fsFile = glCreateShader(GL_FRAGMENT_SHADER);
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
        printf("ERROR TO COMPILE SHADER:\n%s", log);
    } else printf("SHADER COMPILED SUCCESSFULLY\n");

    glDeleteShader(vsFile);
    glDeleteShader(fsFile);

    return shader;
}

void mkr::UnloadShader(Shader &shader){
    if (shader.id != 0){
        glDeleteProgram(shader.id);
        shader.id = 0;
        printf("Shader Unloaded\n");
    }
}

Image mkr::LoadImage(const char *path){
    Image image = {};

    std::vector<unsigned char> bytes = mkr::LoadBytes(path);

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

Texture mkr::LoadTexture(Image &image, type t){
    Texture tex = {};
    GenObject(tex.id, TEX_2D);
    BindObject(tex.id, TEX_2D);
    if (t == NEAREST) mkr::TexParams(GL_TEXTURE_2D, GL_REPEAT, GL_NEAREST);
    if (t == LINEAR) mkr::TexParams(GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR);
    mkr::SetTex2D(GL_RGBA, image.width, image.height, image.data);
    UnloadImage(image);

    return tex;
}

void mkr::UnloadImage(Image &image){
    stbi_image_free(image.data);
}

void mkr::UnloadTexture(Texture &tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("Texture Unloaded\n");
}

void mkr::DrawTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    if (batch.verts.size() + 4  > MAX_VERTS || batch.indices.size() + 6  > MAX_INDICES) mkr::Flush();
    uint32_t base = batch.verts.size();
    uint32_t indexStart = batch.indices.size();

    SetVertsVal(position, size, color);
    SendIndices(base);

    if (batch.calls.empty() || mkr::batch.calls.back().texref != tex){
        batch.calls.push_back({indexStart, 6, tex});
    }
    else batch.calls.back().count += 6;
}

void mkr::DrawSprite(Sprite& sprite, float size, Color color){
	Vec2 tex_size = {
		static_cast<float>(sprite.texture.width * size),
		static_cast<float>(sprite.texture.height * size)
	};

	DrawTexture(&sprite.texture, sprite.position, tex_size, color);
}

void mkr::DrawRectangle(Vec2 position, Vec2 size, Color color){
    uint32_t base = batch.verts.size();
    uint32_t indexStart = batch.indices.size();

    SetVertsVal(position, size, color);
    SendIndices(base);

    if (batch.calls.empty()){
        batch.calls.push_back({indexStart, 6, &core::state.dtex});
    }
    else batch.calls.back().count += 6;
}

void mkr::UnloadMesh(Mesh &mesh){
    mkr::Delete(ARR, mesh.vao);
    mkr::Delete(BUF, mesh.vbo);
    mkr::Delete(BUF, mesh.ebo);
    printf("Mesh Unloaded\n");
}
