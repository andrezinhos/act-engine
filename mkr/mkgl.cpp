#include "mkgl.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

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

char* mkgl::loadShaderFile(const char* path){
    FILE* file = fopen(path, "rb");

    if (!file) return nullptr;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fsize + 1);
    if (!buffer){
        fclose(file);
        return nullptr;
    }

    size_t outsize = fread(buffer, 1, fsize, file);
    buffer[fsize] = '\0';
    fclose(file);

    if (outsize != fsize){
        free(buffer);
        return nullptr;
    }
    return buffer;
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

vertex* mkgl::SetVertex(){
    vertex* verts = (vertex*)malloc(sizeof(vertex) * 4);

    verts[0] = {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
    verts[1] = {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
    verts[2] = {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
    verts[3] = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};

    return verts;
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
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat.v[0][0]);
}

void mkgl::clearScreen(Color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
