#include "mkgl.hpp"
#include "tgt.h"
#include <cstdio>
#include <cstdlib>

char* mkgl::loadShaderFile(const char* path){
    FILE* file = fopen(path, "rb");

    if (!file) return nullptr;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fsize + 1);
    if (!buffer){
        printf("ERROR: shader file couldn't be loaded\n");
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

void getShaderLogInfo(uint32_t* shader, char* log){
    glGetShaderInfoLog(*shader, 512, nullptr, log);
    printf("ERROR ON COMPILING SHADER:\n %s", log);
}

void getProgramLogInfo(uint32_t* prog, char* log){
    glGetProgramInfoLog(*prog, 512, nullptr, log);
    printf("ERROR ON LINKING PROGRAM:\n %s", log);
}

void linkProgram(uint32_t* prog, uint32_t vs, uint32_t fs){
    glAttachShader(*prog, vs);
    glAttachShader(*prog, fs);
    glLinkProgram(*prog);
}

void mkgl::enable_blend(bool flag){
    if (flag){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void mkgl::get_error(){
    GLenum error = glGetError();

    if (error != GL_NO_ERROR){
        switch (error) {
            case 1280: printf("OPENGL ERROR: invalid enum\n"); break;
            case 1281: printf("OPENGL ERROR: invalid value\n"); break;
            case 1282: printf("OPENGL ERROR: invalid operation\n"); break;
            case 1283: printf("OPENGL ERROR: stack overflow\n"); break;
            case 1284: printf("OPENGL ERROR: stack underflow\n"); break;
            case 1285: printf("OPENGL ERROR: out of memory\n"); break;
            case 1286: printf("OPENGL ERROR: invalid framebuffer operation\n"); break;
        }
    }
}

bool mkgl::gen_array(uint32_t* obj){
    glGenVertexArrays(1, obj);

    if (*obj == 0){
        printf("ERROR: failed to gen vertex array\n");
        return false;
    }

    return true;
}

bool mkgl::gen_buffer(uint32_t* obj){
    glGenBuffers(1, obj);

    if (!obj){
        printf("ERROR: failed to gen buffer\n");
        return false;
    }

    return true;
}

bool mkgl::create_array(uint32_t *v){
    glCreateVertexArrays(1, v);

    if (*v == 0) {
        printf("ERROR: failed to create vertex array\n");
        return false;
    }

    return true;
}

bool mkgl::create_buffer(uint32_t *vo){
    glCreateBuffers(1, vo);

    if (*vo == 0) {
        printf("ERROR: failed to create buffer\n");
        return false;
    }

    return true;
}

void mkgl::element_connect(uint32_t *vao, uint32_t *vo){
    glVertexArrayElementBuffer(*vao, *vo);
}

void mkgl::buf_data_static(uint32_t* buffer, const void* data, size_t size){
    glNamedBufferData(*buffer, size, data, GL_STATIC_DRAW);
}

void mkgl::buf_data_dynamic(uint32_t* buffer, const void* data, size_t size){
    glNamedBufferData(*buffer, size, data, GL_DYNAMIC_DRAW);
}

void mkgl::buf_sub_data(uint32_t *buffer, const void *data, size_t size){
    glNamedBufferSubData(*buffer, 0, size, data);
}

void mkgl::bind_arr_buf(uint32_t* vo){
    glBindVertexArray(*vo);
}

void mkgl::bind_buf(uint32_t* vo){
    glBindBuffer(GL_ARRAY_BUFFER, *vo);
}

void mkgl::buf_storage(int target, size_t size, void* data, GLbitfield flags){
    glBufferStorage(target, size, data, flags);
}

void* mkgl::map_buffer(int target, size_t size, GLbitfield flags){
    return glMapBufferRange(target, 0, size, flags);
}

void mkgl::arr_unbind(){
    glBindVertexArray(0);
}

void mkgl::buf_unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//GL_ARRAY_BUFFER
//GL_ELEMENT_ARRAY_BUFFER
void mkgl::bind_data_static(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
}

void mkgl::bind_data_dynamic(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferData(target, size, data, GL_DYNAMIC_DRAW);
}

void mkgl::bind_sub_data(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferSubData(target, 0, size, data);
}

void mkgl::sendAttribPtr(int layout, int size, int stride, int ptr){
    glVertexAttribPointer(
        layout, size,
        GL_FLOAT, GL_FALSE,
        stride * sizeof(float),
        (void*)(ptr * sizeof(float))
    );
    glEnableVertexAttribArray(layout);
}

void mkgl::set_attrib_vertex_buffer(uint32_t *vao, uint32_t *vbo, int stride){
    glVertexArrayVertexBuffer(*vao, 0, *vbo, 0, stride * sizeof(float));
}

void mkgl::set_attrib_format(uint32_t *vao, int layout, int size, int offset){
    glVertexArrayAttribFormat(
        *vao, layout, size,
        GL_FLOAT, GL_FALSE,
        offset * sizeof(float)
    );
}

void mkgl::enable_array_attrib(uint32_t *vao, int layout){
    glEnableVertexArrayAttrib(*vao, layout);
}

void mkgl::bind_vertex_attrib(uint32_t *vao, int attrib, int layout){
    glVertexArrayAttribBinding(*vao, layout, attrib);
}

// std::vector<vertex> mkgl::SetNDC(){
//     return {
//         {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
//         {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
//         {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//         {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
//     };
// }

void mkgl::set_initial_vertex(vertex* data){
    data[0] = {{ 0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
    data[1] = {{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
    data[2] = {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
    data[3] = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
}

bool getShaderError(uint32_t* shader){
    int pass;
    char log[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &pass);

    if (!pass){
        getShaderLogInfo(shader, log);
        return false;
    }

    return true;
}

bool getShaderProgError(uint32_t* prog){
    int pass;
    char log[512];
    glGetProgramiv(*prog, GL_LINK_STATUS, &pass);

    if (!pass){
        getProgramLogInfo(prog, log);
        return false;
    }

    return true;
}

bool mkgl::create_shader(uint32_t* sh, const char* src, int target){
    *sh = glCreateShader(target);
    glShaderSource(*sh, 1, &src, nullptr);
    glCompileShader(*sh);
    return getShaderError(sh);
}

bool mkgl::gen_shader_prog(uint32_t* prog, uint32_t vs, uint32_t fs){
    *prog = glCreateProgram();
    linkProgram(prog, vs, fs);
    return getShaderProgError(prog);
}

void mkgl::delete_shaders(uint32_t vs, uint32_t fs){
    if (vs != 0) glDeleteShader(vs);
    if (fs != 0) glDeleteShader(fs);
}

void mkgl::setUniformMat(GLint loc, Matrix* mat){
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat->v[0][0]);
}

void mkgl::clearScreen(color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void mkgl::del_prog(uint32_t* obj){
    if (obj != 0) glDeleteProgram(*obj);
}

void mkgl::del_arr(uint32_t* obj){
    if (obj != 0) glDeleteVertexArrays(1, obj);
}

void mkgl::del_buffer(uint32_t* obj){
    if (obj != 0) glDeleteBuffers(1, obj);
}
