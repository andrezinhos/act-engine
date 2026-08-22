#include "mkr.hpp"
#include "core.hpp"
#include "utils.hpp"
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr size_t MAX_VERTS = 1000;
constexpr size_t MAX_INDICES = 1500;

size_t VSIZE(size_t size){ return size * sizeof(Verts); }
size_t ISIZE(size_t size){ return size * sizeof(uint32_t); }

Batch mkr::batch = {};

std::vector<uint8_t> mkr::LoadBytes(const char* path){
    std::ifstream file(path, std::ios::binary);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    return buffer;
}

std::string mkr::LoadShaderFile(const char* path){
    std::ifstream file(path, std::ios::binary);

    std::ostringstream buffer;
    buffer << file.rdbuf();

    std::string shaderFile = buffer.str();
    return shaderFile;
}

bool mkr::ShaderError(uint32_t& src){
    int pass;
    char log[512];
    glGetShaderiv(src, GL_COMPILE_STATUS, &pass);

    if (!pass){
        glGetShaderInfoLog(src, 512, nullptr, log);
        printf("Error in compiling shader:\n%s", log);
        return false;
    }

    return true;
}

std::vector<Verts> mkr::SetVertsVal(Vec2 position, Vec2 size, Color color){
    return {
        {{ position.x,        	position.y,          0.0f },{ color.r, color.g, color.b }, { 0.0f, 0.0f }},
        {{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { 1.0f, 0.0f }},
        {{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 1.0f, 1.0f }},
        {{ position.x,        	position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 0.0f, 1.0f }}
    };
}

std::vector<Verts> mkr::SetVertsVal(Vec3 position, Vec2 size, Color color){
    return {
        {{ position.x,        	position.y,          position.z },{ color.r, color.g, color.b }, { 0.0f, 0.0f }},
        {{ position.x + size.x, position.y,          position.z },{ color.r, color.g, color.b }, { 1.0f, 0.0f }},
        {{ position.x + size.x, position.y + size.y, position.z },{ color.r, color.g, color.b }, { 1.0f, 1.0f }},
        {{ position.x,        	position.y + size.y, position.z },{ color.r, color.g, color.b }, { 0.0f, 1.0f }}
    };
}

std::vector<Verts> mkr::SetStdNDC(){
    return {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
}

void mkr::SendIndices(uint32_t base){
    batch.indices.push_back(base + 0);
    batch.indices.push_back(base + 1);
    batch.indices.push_back(base + 3);

    batch.indices.push_back(base + 1);
    batch.indices.push_back(base + 2);
    batch.indices.push_back(base + 3);
}

void mkr::TextureActive(int num){
    if (num == 0) glActiveTexture(GL_TEXTURE0);
    if (num == 1) glActiveTexture(GL_TEXTURE1);
}

void mkr::Flush(){
    if (batch.verts.empty()) return;

    BindObject(batch.vao, ARR);

    BindObject(batch.vbo, BUF);
    BindData(
        BUF, batch.verts.size() * sizeof(Verts),
        batch.verts.data(), DYNAMIC
    );

    BindObject(batch.ebo, ELM);
    BindData(
        ELM, batch.indices.size() * sizeof(uint32_t),
        batch.indices.data(), DYNAMIC
    );

    for (const auto& d : batch.calls){
        glActiveTexture(GL_TEXTURE0);

        BindObject(d.texref->id, TEX_2D);

        FigureDraw(
            ELM,
            d.count,
            (void*)(d.start * sizeof(uint32_t))
        );
    }

    batch.verts.clear();
    batch.indices.clear();
    batch.calls.clear();
}

void mkr::FigureDraw(type t, size_t size, void* offset){
    if (t == ARR) glDrawArrays(GL_TRIANGLES, 0, size);
    if (t == ELM) glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, offset);
}

void mkr::SetUniform(GLint type, Matrix &mat){
    glUniformMatrix4fv(type, 1, GL_FALSE, &mat.m[0][0]);
}

void mkr::GetUniformLoc(Shader& shader, type t, const char* name){
    if (t == PROJ) shader.uproj = glGetUniformLocation(shader.id, name);
    if (t == VIEW) shader.uview = glGetUniformLocation(shader.id, name);
    if (t == MODEL) shader.umodel = glGetUniformLocation(shader.id, name);
    if (t == TEX_2D) shader.utex = glGetUniformLocation(shader.id, name);
}

void mkr::GenObject(uint32_t& vo, type t){
    if (t == ARR) glGenVertexArrays(1, &vo);
    if (t == BUF) glGenBuffers(1, &vo);
    if (t == TEX_2D) glGenTextures(1, &vo);
}

void mkr::BindObject(uint32_t& vo, type t){
    if (t == ARR) glBindVertexArray(vo);
    if (t == BUF) glBindBuffer(GL_ARRAY_BUFFER, vo);
    if (t == ELM) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo);
    if (t == TEX_2D) glBindTexture(GL_TEXTURE_2D, vo);
}

void mkr::BindObject(int vo, type t){
    if (t == ARR) glBindVertexArray(vo);
    if (t == BUF) glBindBuffer(GL_ARRAY_BUFFER, vo);
    if (t == ELM) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo);
}

void mkr::BindData(type t, size_t size, const void* data, type draw){
    GLenum drawMode;
    if (draw == STATIC) drawMode = GL_STATIC_DRAW;
    if (draw == DYNAMIC) drawMode = GL_DYNAMIC_DRAW;

    if (t == BUF) glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
    if (t == ELM) glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawMode);
}

void mkr::BindSubData(type t, size_t size, const void* data){
    if (t == BUF) glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    if (t == ELM) glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void mkr::AttribPtr(int loc, int locSize, int size, int ptr){
    glVertexAttribPointer(loc, locSize, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(ptr * sizeof(float)));
    glEnableVertexAttribArray(loc);
}

void mkr::TexParams(GLenum form, GLenum texstyle, GLenum map){
    glTexParameteri(form, GL_TEXTURE_WRAP_S, texstyle);
    glTexParameteri(form, GL_TEXTURE_WRAP_T, texstyle);
    glTexParameteri(form, GL_TEXTURE_MIN_FILTER, map);
    glTexParameteri(form, GL_TEXTURE_MAG_FILTER, map);
}

void mkr::SetTex2D(GLenum color, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, color, width, height, 0, color, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Shader mkr::DefaultShader(){
    Shader shader = {};
    std::string vsPath = LoadShaderFile("assets/shaders/simple.vert");
    std::string fsPath = LoadShaderFile("assets/shaders/simple.frag");

    const char* srcvs = vsPath.c_str();
    const char* srcfs = fsPath.c_str();

    uint32_t vsFile = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsFile, 1, &srcvs, nullptr);
    glCompileShader(vsFile);
    ShaderError(vsFile);

    uint32_t fsFile = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsFile, 1, &srcfs, nullptr);
    glCompileShader(fsFile);
    ShaderError(fsFile);

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
    } else printf("[INFO] DEFAULT SHADER COMPILED\n");

    GetUniformLoc(shader, PROJ, "uProj");
    GetUniformLoc(shader, VIEW, "uView");
    GetUniformLoc(shader, MODEL, "uModel");
    GetUniformLoc(shader, TEX_2D, "uTexture");

    Delete(SHAD, vsFile);
    Delete(SHAD, fsFile);

    return shader;
}

Texture mkr::DefaultTexture(){
    Texture tex;
    GenObject(tex.id, TEX_2D);
    BindObject(tex.id, TEX_2D);
    uint32_t color = 0xFFFFFFFF;
    SetTex2D(GL_RGBA, 1, 1, &color);
    return tex;
}

Mesh mkr::DefaultQuad(){
    Mesh mesh;
    mesh.vertices = SetStdNDC();
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    TemplateBuf(
        mesh.vao, mesh.vbo, mesh.ebo,
        mesh.vertices.data(), mesh.indices.data(),
        VSIZE(mesh.vertices.size()), ISIZE(mesh.indices.size()),
        STATIC
    );
    return mesh;
}

void mkr::DefaultBatch(){
    TemplateBuf(
        batch.vao, batch.vbo, batch.ebo,
        nullptr, nullptr,
        VSIZE(MAX_VERTS), ISIZE(MAX_INDICES),
        DYNAMIC
    );
}

void mkr::Delete(type t, uint32_t &obj){
    if (t == ARR && obj != 0) glDeleteVertexArrays(1, &obj);
    if (t == BUF && obj != 0) glDeleteBuffers(1, &obj);
    if (t == TEX_2D && obj != 0) glDeleteTextures(1, &obj);
    if (t == PROG && obj != 0) glDeleteProgram(obj);
    if (t == SHAD && obj != 0) glDeleteShader(obj);
}

/* RENDER IMPLEMENTATION */

Shader mkr::LoadShader(const char *vs, const char *fs){
    Shader shader = {};
    std::string vsPath = mkr::LoadShaderFile(vs);
    std::string fsPath = mkr::LoadShaderFile(fs);

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

void mkr::UnloadShader(Shader &shader){
    if (shader.id != 0){
        glDeleteProgram(shader.id);
        shader.id = 0;
        printf("Shader Unloaded\n");
    }
}

Image mkr::LoadImage(const char *path){
    Image image = {};

    std::vector<uint8_t> bytes = mkr::LoadBytes(path);

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
    uint32_t base = mkr::batch.verts.size();
    uint32_t indexStart = mkr::batch.indices.size();

    auto verts = mkr::SetVertsVal(position, size, color);
    mkr::SendIndices(base);

    for (const auto& v : verts) mkr::batch.verts.push_back(v);

    if (tex == nullptr) tex = &core::state.dtex;
    if (mkr::batch.calls.empty() || mkr::batch.calls.back().texref != tex){
        mkr::batch.calls.push_back({indexStart, 6, tex});
    }
}

void mkr::TemplateBuf(uint32_t& vao, uint32_t& vbo, const void* data, size_t size, type draw){
    GenObject(vao, ARR);
    GenObject(vbo, BUF);
    BindObject(vao, ARR);

    BindObject(vbo, BUF);
    BindData(BUF, size, data, draw);

    AttribPtr(0, 3, 8, 0);
    AttribPtr(1, 3, 8, 3);
    AttribPtr(2, 2, 8, 6);

    BindObject(0, ARR);
}

void mkr::TemplateBuf(uint32_t& vao, uint32_t& vbo, uint32_t& ebo, const void* data,
    const void* datai, size_t size, size_t sizei, type draw
){
    GenObject(vao, ARR);
    GenObject(vbo, BUF);
    GenObject(ebo, BUF);

    BindObject(vao, ARR);

    BindObject(vbo, BUF);
    BindData(BUF, size, data, draw);

    BindObject(ebo, ELM);
    BindData(ELM, sizei, datai, draw);

    AttribPtr(0, 3, 8, 0);
    AttribPtr(1, 3, 8, 3);
    AttribPtr(2, 2, 8, 6);

    BindObject(0, ARR);
}

void mkr::DrawRectangle(Vec2 position, float size, Color color){
    uint32_t base = mkr::batch.verts.size();
    uint32_t indexStart = mkr::batch.indices.size();

    auto verts = mkr::SetVertsVal(position, {size, size}, color);

    for (const auto& v : verts) mkr::batch.verts.push_back(v);
    mkr::SendIndices(base);

    if (mkr::batch.calls.empty()){
        mkr::batch.calls.push_back({indexStart, 6, &core::state.dtex});
    }
}

void mkr::UnloadMesh(Mesh &mesh){
    mkr::Delete(ARR, mesh.vao);
    mkr::Delete(BUF, mesh.vbo);
    mkr::Delete(BUF, mesh.ebo);
    printf("Mesh Unloaded\n");
}
