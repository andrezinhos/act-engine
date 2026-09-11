#include "mkgl.hpp"
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_GIF
#define STBI_NO_PNM
#define STBI_NO_BMP
#define STBI_NO_PIC
#define STBI_NO_PSD
#define STBI_NO_HDR
#include "stb_image.h"
#include <cstdio>
#include <cstdlib>

void mkgl::freeptr(void* ptr){
    free(ptr);
    ptr = nullptr;
}

byte* mkgl::loadBytes(const char* path, size_t* size){
    FILE* file = fopen(path, "rb");

    if (!file) return nullptr;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    byte* buffer = (byte*)malloc(fsize);
    if (!buffer){
        perror("Error to load File");
        fclose(file);
        return nullptr;
    }

    size_t outsize = fread(buffer, 1, fsize, file);
    fclose(file);

    if (outsize != fsize){
        freeptr(buffer);
        return nullptr;
    }
    
    if (size) *size = outsize;
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

void mkgl::getShaderLogInfo(uint* shader, char* log){
    glGetShaderInfoLog(*shader, 512, nullptr, log);
    printf("ERROR ON COMPILING SHADER:\n %s", log);
}

void mkgl::getProgramLogInfo(uint* prog, char* log){
    glGetProgramInfoLog(*prog, 512, nullptr, log);
    printf("ERROR ON LINKING PROGRAM:\n %s", log);
}

void mkgl::linkProgram(uint* prog, uint vs, uint fs){
    glAttachShader(*prog, vs);
    glAttachShader(*prog, fs);
    glLinkProgram(*prog);
}

void mkgl::enableBlend(bool flag){
    if (flag){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void mkgl::genArrayBuffer(uint* obj){
    glGenVertexArrays(1, obj);
}

void mkgl::genBuffer(uint* obj){
    glGenBuffers(1, obj);
}

void mkgl::bindArrBuff(uint* vo){
    glBindVertexArray(*vo);
}

//GL_ARRAY_BUFFER
//GL_ELEMENT_ARRAY_BUFFER
void mkgl::bindBuff(uint* vo, GLenum type){
    glBindBuffer(type, *vo);
}

void mkgl::unbind(){
    glBindVertexArray(0);
}

//GL_ARRAY_BUFFER
//GL_ELEMENT_ARRAY_BUFFER
void mkgl::bindDataStatic(GLenum type, const void* data, size_t size){
    glBufferData(type, size, data, GL_STATIC_DRAW);
}

void mkgl::bindDataDynamic(GLenum type, const void* data, size_t size){
    glBufferData(type, size, data, GL_DYNAMIC_DRAW);
}

void mkgl::bindSubData(GLenum type, const void *data, size_t size){
    glBufferSubData(type, 0, size, data);
}

void mkgl::sendAttribPtr(int layout, int size, int stride, int ptr){
    glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(ptr * sizeof(float)));
    glEnableVertexAttribArray(layout);
}

std::vector<vertex> mkgl::SetNDC(){
    return {
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };
}

bool mkgl::getShaderError(uint* shader){
    int pass;
    char log[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &pass);

    if (!pass){
        getShaderLogInfo(shader, log);
        return false;
    }

    return true;
}

bool mkgl::getShaderProgError(uint* prog){
    int pass;
    char log[512];
    glGetProgramiv(*prog, GL_LINK_STATUS, &pass);

    if (!pass){
        getProgramLogInfo(prog, log);
        return false;
    }

    return true;
}

uint mkgl::genShader(const char* src, GLenum type){
    uint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    return sh;
}

bool mkgl::compileShader(uint shader){
    glCompileShader(shader);
    return getShaderError(&shader);
}

void mkgl::genShaderProg(uint* prog, uint vs, uint fs){
    *prog = glCreateProgram();
    linkProgram(prog, vs, fs);
    getShaderProgError(prog);
}

void mkgl::deleteShaders(uint vs, uint fs){
    glDeleteShader(vs);
    glDeleteShader(fs);
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

    size_t imgsize = 0;
    byte* imgbuf = loadBytes(path, &imgsize);

    image.data = stbi_load_from_memory(
        imgbuf,
        imgsize,
        &image.width,
        &image.height,
        &image.channels,
        4
    );

    freeptr(imgbuf);
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

void mkgl::setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void mkgl::deleteProg(uint* obj){
    if (obj != 0) glDeleteProgram(*obj);
}

void mkgl::deleteVertexArr(uint* obj){
    if (obj != 0) glDeleteVertexArrays(1, obj);
}

void mkgl::deleteBuffer(uint* obj){
    if (obj != 0) glDeleteBuffers(1, obj);
}
