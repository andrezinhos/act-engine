#include "mkgl.hpp"
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

DState mkgl::state = {};

std::vector<byte> mkgl::loadBytes(const char* path){
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) return {};

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<byte> buffer(size);

    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    file.close();

    return buffer;
}

std::string mkgl::loadShaderFile(const std::string& path){
    std::ifstream file(path, std::ios::binary);

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string types(size, '\0');
    file.read(&types[0], size);

    return types;
}

uint mkgl::genBuff(types b){
    uint obj;
    if (b == types::arr) glGenVertexArrays(1, &obj);
    if (b == types::buff || b == types::element) glGenBuffers(1, &obj);

    return obj;
}

void mkgl::bindBuff(uint& vo, types b){
    if (b == types::arr) glBindVertexArray(vo);
    if (b == types::buff) glBindBuffer(GL_ARRAY_BUFFER, vo);
    if (b == types::element) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo);
}

void mkgl::unbind(){
    glBindVertexArray(0);
}

void mkgl::bindDataStatic(types b, const void* data, size_t size){
    if (b == types::buff) glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    if (b == types::element) glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void mkgl::bindDataDynamic(types b, const void* data, size_t size){
    if (b == types::buff) glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    if (b == types::element) glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void mkgl::bindSubData(types b, const void *data, size_t size){
    if (b == types::buff) glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    if (b == types::element) glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}

void mkgl::sendAttribPtr(int layout, int size, int stride, int ptr){
    glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(ptr * sizeof(float)));
    glEnableVertexAttribArray(layout);
}

std::vector<vertex> mkgl::SetNDC(){
    return {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
}

void mkgl::drawElements(size_t count, void* offset){
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, offset);
}

void mkgl::flush(){
    if (mkgl::state.dbatch.vertices.empty()) return;

    bindBuff(mkgl::state.dbatch.vao, types::arr);

    bindBuff(mkgl::state.dbatch.vbo, types::buff);
    bindSubData(
        types::buff, state.dbatch.vertices.data(),
        state.dbatch.vertices.size() * sizeof(vertex)
    );

    bindBuff(mkgl::state.dbatch.ebo, types::element);
    bindSubData(
        types::element, state.dbatch.indices.data(),
        state.dbatch.indices.size() * sizeof(uint32_t)
    );

    for (const auto& d : state.dbatch.calls){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            (void*)(d.start * sizeof(uint))
        );
    }

    state.dbatch.calls.clear();
    state.dbatch.indices.clear();
    state.dbatch.vertices.clear();
}

bool mkgl::getShaderError(uint& shader){
    int pass;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &pass);

    if (!pass){
        glGetShaderInfoLog(shader, 512, nullptr, log);
        printf("ERROR ON COMPILING SHADER:\n %s", log);
        return false;
    }

    return true;
}

bool mkgl::getShaderProgError(uint& prog){
    int pass;
    char log[512];
    glGetProgramiv(prog, GL_LINK_STATUS, &pass);

    if (!pass){
        glGetProgramInfoLog(prog, 512, nullptr, log);
        printf("ERROR TO CREATE SHADER PROGRAM:\n %s", log);
        return false;
    }

    return true;
}

uint mkgl::genShader(const char* src, GLenum type){
    uint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    return sh;
}

bool mkgl::compileShader(uint& shader){
    glCompileShader(shader);
    return getShaderError(shader);
}

uint mkgl::genShaderProg(uint& vs, uint& fs){
    uint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    getShaderProgError(prog);
    return prog;
}

void mkgl::setUniformMat(GLint loc, Matrix& mat){
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat.at(0, 0));
}

void mkgl::clearScreen(Color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Image mkgl::loadImage(const char* path){
    Image image = {};

    std::vector<byte> imgbuf = loadBytes(path);

    image.data = stbi_load_from_memory(
        imgbuf.data(),
        imgbuf.size(),
        &image.width,
        &image.height,
        &image.channels,
        4
    );

    return image;
}

void mkgl::unloadImage(Image& image){
    stbi_image_free(image.data);
}

uint mkgl::genTex(GLenum type){
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(type, tex);
    return tex;
}

void mkgl::setTexParams(GLenum type, GLenum wrap, GLenum format){
    glTexParameteri(type, wrap, format);
}

void mkgl::setTexImage2D(GLenum format, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void mkgl::Delete(uint& obj, types t){
    if (obj != 0){
        if (t == types::arr) glDeleteVertexArrays(1, &obj);
        if (t == types::buff || t == types::element) glDeleteBuffers(1, &obj);
        if (t == types::prog) glDeleteProgram(obj);
    }
}
