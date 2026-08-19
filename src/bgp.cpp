#include "bgp.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

constexpr size_t MAX_VERTS = 1000;
constexpr size_t MAX_INDICES = 1500;

std::vector<uint8_t> bgp::LoadBytes(const char* path){
    std::ifstream file(path, std::ios::binary);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    return buffer;
}

std::string bgp::LoadShaderFile(const char* path){
    std::ifstream file(path, std::ios::binary);

    std::ostringstream buffer;
    buffer << file.rdbuf();

    std::string shaderFile = buffer.str();
    return shaderFile;
}

bool bgp::ShaderError(uint32_t& src){
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

std::vector<Verts> bgp::SetVertsVal(Vec2 position, Vec2 size, Color color){
    return {
        {{ position.x,        	position.y,          0.0f },{ color.r, color.g, color.b }, { 0.0f, 0.0f }},
        {{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { 1.0f, 0.0f }},
        {{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 1.0f, 1.0f }},
        {{ position.x,        	position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 0.0f, 1.0f }}
    };
}

std::vector<Verts> bgp::SetVertsVal(Vec3 position, Vec2 size, Color color){
    return {
        {{ position.x,        	position.y,          position.z },{ color.r, color.g, color.b }, { 0.0f, 0.0f }},
        {{ position.x + size.x, position.y,          position.z },{ color.r, color.g, color.b }, { 1.0f, 0.0f }},
        {{ position.x + size.x, position.y + size.y, position.z },{ color.r, color.g, color.b }, { 1.0f, 1.0f }},
        {{ position.x,        	position.y + size.y, position.z },{ color.r, color.g, color.b }, { 0.0f, 1.0f }}
    };
}

std::vector<Verts> bgp::SetStdNDC(){
    return {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
}

void bgp::SendIndices(uint32_t base){
    batch.indices.push_back(base + 0);
    batch.indices.push_back(base + 1);
    batch.indices.push_back(base + 3);

    batch.indices.push_back(base + 1);
    batch.indices.push_back(base + 2);
    batch.indices.push_back(base + 3);
}

void bgp::Flush(){
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
            d.count,
            (void*)(d.start * sizeof(uint32_t))
        );
    }

    batch.verts.clear();
    batch.indices.clear();
    batch.calls.clear();
}

void bgp::FigureDraw(size_t size, void* offset){
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, offset);
}

Matrix bgp::GetViewMatrix(Camera2D& cam){
    Matrix t = gmath::TranslateMatrix({-cam.position.x, -cam.position.y, 0.0f});
    Matrix r = gmath::MatrixRotateZ(-cam.rotation);
    Matrix s = gmath::ScaleMatrix({cam.zoom, cam.zoom, 1.0});

    return gmath::MultiplyMatrix(gmath::MultiplyMatrix(s, r), t);
}

Matrix bgp::GetProjectionMatrix(int width, int height){
    return gmath::OrthoMatrix(
        0.0f, static_cast<float>(width),
        0.0f, static_cast<float>(height)
    );
}

void bgp::SetUniform(GLint type, Matrix &mat){
    glUniformMatrix4fv(type, 1, GL_FALSE, mat.m[0]);
}

void bgp::GetUniformLoc(Shader& shader, type t, const char* name){
    if (t == PROJ) shader.uproj = glGetUniformLocation(shader.id, name);
    if (t == VIEW) shader.uview = glGetUniformLocation(shader.id, name);
    if (t == TEX_2D) shader.utex = glGetUniformLocation(shader.id, name);
}

void bgp::CreateObject(uint32_t& vo, type t){
    if (t == ARR) glGenVertexArrays(1, &vo);
    if (t == BUF) glGenBuffers(1, &vo);
}

void bgp::BindObject(uint32_t& vo, type t){
    if (t == ARR) glBindVertexArray(vo);
    if (t == BUF) glBindBuffer(GL_ARRAY_BUFFER, vo);
    if (t == ELM) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo);
    if (t == TEX_2D) glBindTexture(GL_TEXTURE_2D, vo);
}

void bgp::BindObject(int vo, type t){
    if (t == ARR) glBindVertexArray(vo);
    if (t == BUF) glBindBuffer(GL_ARRAY_BUFFER, vo);
    if (t == ELM) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo);
}

void bgp::BindData(type t, size_t size, const void* data, type draw){
    GLenum drawMode;
    if (draw == STATIC) drawMode = GL_STATIC_DRAW;
    if (draw == DYNAMIC) drawMode = GL_DYNAMIC_DRAW;

    if (t == BUF) glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
    if (t == ELM) glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawMode);
}

void bgp::BindSubData(type t, size_t size, const void* data){
    if (t == BUF) glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    if (t == ELM) glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void bgp::AttribPtr(int loc, int locSize, int size, int ptr){
    glVertexAttribPointer(loc, locSize, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(ptr * sizeof(float)));
    glEnableVertexAttribArray(loc);
}

void bgp::TexParams(GLenum form, GLenum texstyle, GLenum map){
    glTexParameteri(form, GL_TEXTURE_WRAP_S, texstyle);
    glTexParameteri(form, GL_TEXTURE_WRAP_T, texstyle);
    glTexParameteri(form, GL_TEXTURE_MIN_FILTER, map);
    glTexParameteri(form, GL_TEXTURE_MAG_FILTER, map);
}

void bgp::GenTexture(uint32_t& id){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
}

void bgp::SetTex2D(GLenum color, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, color, width, height, 0, color, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Shader bgp::DefaultShader(){
    Shader shader = {};
    std::string vsPath = LoadShaderFile("simple.vert");
    std::string fsPath = LoadShaderFile("simple.frag");

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
    } else printf("[INFO] DEFAULT SHADER COMPILED\n");

    GetUniformLoc(shader, PROJ, "uProj");
    GetUniformLoc(shader, VIEW, "uView");
    GetUniformLoc(shader, TEX_2D, "uTexture");

    Delete(SHAD, vsFile);
    Delete(SHAD, fsFile);

    return shader;
}

Texture bgp::DefaultTexture(){
    Texture tex;
    GenTexture(tex.id);
    uint32_t color = 0xFFFFFFFF;
    SetTex2D(GL_RGBA, 1, 1, &color);
    return tex;
}

Mesh bgp::DefaultQuad(){
    Mesh mesh;

    mesh.vertices = SetStdNDC();
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    CreateObject(mesh.vao, ARR);
    CreateObject(mesh.vbo, BUF);
    CreateObject(mesh.ebo, BUF);

    BindObject(mesh.vao, ARR);

    BindObject(mesh.vbo, BUF);
    BindData(
        BUF,
        mesh.vertices.size() * sizeof(Verts),
        mesh.vertices.data(), STATIC
    );

    BindObject(mesh.ebo, ELM);
    BindData(
        ELM,
        mesh.indices.size() * sizeof(uint32_t),
        mesh.indices.data(), STATIC
    );

    AttribPtr(0, 3, 8, 0);
    AttribPtr(1, 3, 8, 3);
    AttribPtr(2, 2, 8, 6);

    return mesh;
}

void bgp::DefaultBatch(){
    CreateObject(batch.vao, ARR);
    CreateObject(batch.vbo, BUF);
    CreateObject(batch.ebo, BUF);

    BindObject(batch.vao, ARR);

    BindObject(batch.vbo, BUF);
    BindData(BUF, MAX_VERTS * sizeof(Verts), nullptr, DYNAMIC);

    BindObject(batch.ebo, ELM);
    BindData(ELM, MAX_INDICES * sizeof(uint32_t), nullptr, DYNAMIC);

    AttribPtr(0, 3, 8, 0);
    AttribPtr(1, 3, 8, 3);
    AttribPtr(2, 2, 8, 6);
}

void bgp::Delete(type t, uint32_t &obj){
    if (t == ARR && obj != 0) glDeleteVertexArrays(1, &obj);
    if (t == BUF && obj != 0) glDeleteBuffers(1, &obj);
    if (t == TEX_2D && obj != 0) glDeleteTextures(1, &obj);
    if (t == PROG && obj != 0) glDeleteProgram(obj);
    if (t == SHAD && obj != 0) glDeleteShader(obj);
}
