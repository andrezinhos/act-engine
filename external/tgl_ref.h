#pragma once
#include "glad.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tgt.h"

#define TGL_POSITION_LAYOUT 0
#define TGL_COLOR_LAYOUT 1
#define TGL_TEXTURE_LAYOUT 2
#define TGL_VERTEX_STRIDE 8

#define VMAX 1004
#define IMAX 1506
#define CMAX 512

#ifdef TGL_STATIC
    #define TGLDEF static
#else
    #define TGLDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void tgl_enable_blend(int flag);

extern int tgl_gen_array(uint32_t* obj);
extern int tgl_gen_buffer(uint32_t* obj);

extern int tgl_create_buffer(uint32_t* vo);
extern int tgl_create_array(uint32_t* v);

extern void tgl_element_connect(uint32_t* vao, uint32_t* vo);

extern void tgl_buf_data_static(uint32_t* buffer, const void* data, size_t size);
extern void tgl_buf_data_dynamic(uint32_t* buffer, const void* data, size_t size);
extern void tgl_buf_sub_data(uint32_t* buffer, const void* data, size_t size);
extern void tgl_buf_storage(int target, size_t size, void* data, GLbitfield flags);
extern void* tgl_map_buffer(int target, size_t size, GLbitfield flags);

extern void tgl_arr_unbind();
extern void tgl_buf_unbind();

extern void tgl_bind_arr_buf(uint32_t* vo);
extern void tgl_bind_buf(uint32_t* vo);
extern void tgl_bind_data_static(uint32_t* buffer, const void* data, size_t size, int target);
extern void tgl_bind_data_dynamic(uint32_t* buffer, const void* data, size_t size, int target);
extern void tgl_bind_sub_data(uint32_t* buffer, const void* data, size_t size, int target);

// extern  void sendMapAttribPtr(int layout, int locSize, int stride, int ptr);
extern void tgl_send_attrib(int layout, int locSize, int stride, int ptr);

extern void tgl_attrib_vertex_buffer(uint32_t* vao, uint32_t* vbo, int stride);
extern void tgl_attrib_arr_format(uint32_t*vao, int layout, int size, int offset);
extern void tgl_enable_array_attrib(uint32_t* vao, int layout);
extern void tgl_bind_vertex_attrib(uint32_t* vao, int attrib, int layout);

extern char* tgl_load_shader_file(const char* path);
extern void tgl_set_initial_vertex(vertex* data);

extern int tgl_create_shader(uint32_t* sh, const char* src, int target);
extern int tgl_gen_shader_prog(uint32_t* prog, uint32_t* vs, uint32_t* fs);
extern void tgl_delete_shaders(uint32_t vs, uint32_t fs);
extern void tgl_set_uniform(int loc, float* mat);
extern void tgl_clear_screen(color color);

extern void tgl_del_arr(uint32_t* obj);
extern void tgl_del_buffer(uint32_t* obj);
extern void tgl_del_prog(uint32_t* obj);

#ifdef TGL_IMPL

extern char* tgl_load_shader_file(const char* path){
    FILE* file = fopen(path, "rb");

    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fsize + 1);
    if (!buffer){
        printf("ERROR: shader file couldn't be loaded\n");
        fclose(file);
        return NULL;
    }

    size_t outsize = fread(buffer, 1, fsize, file);
    buffer[fsize] = '\0';
    fclose(file);

    if (outsize != fsize){
        free(buffer);
        return NULL;
    }
    return buffer;
}

extern void getShaderLogInfo(uint32_t* shader, char* log){
    glGetShaderInfoLog(*shader, 512, NULL, log);
    printf("ERROR ON COMPILING SHADER:\n %s", log);
}

extern void getProgramLogInfo(uint32_t* prog, char* log){
    glGetProgramInfoLog(*prog, 512, NULL, log);
    printf("ERROR ON LINKING PROGRAM:\n %s", log);
}

extern void linkProgram(uint32_t* prog, uint32_t vs, uint32_t fs){
    glAttachShader(*prog, vs);
    glAttachShader(*prog, fs);
    glLinkProgram(*prog);
}

extern void tgl_enable_blend(int flag){
    if (flag == 1){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

extern int tgl_gen_array(uint32_t* obj){
    glGenVertexArrays(1, obj);

    if (*obj == 0){
        printf("ERROR: failed to gen vertex array\n");
        return 1;
    }

    return 0;
}

extern int tgl_gen_buffer(uint32_t* obj){
    glGenBuffers(1, obj);

    if (!obj){
        printf("ERROR: failed to gen buffer\n");
        return 1;
    }

    return 0;
}

extern int tgl_create_array(uint32_t *v){
    glCreateVertexArrays(1, v);

    if (*v == 0) {
        printf("ERROR: failed to create vertex array\n");
        return 1;
    }

    return 0;
}

extern int tgl_create_buffer(uint32_t *vo){
    glCreateBuffers(1, vo);

    if (*vo == 0) {
        printf("ERROR: failed to create buffer\n");
        return 1;
    }

    return 0;
}

extern void tgl_element_connect(uint32_t *vao, uint32_t *vo){
    glVertexArrayElementBuffer(*vao, *vo);
}

extern void tgl_buf_data_static(uint32_t* buffer, const void* data, size_t size){
    glNamedBufferData(*buffer, size, data, GL_STATIC_DRAW);
}

extern void tgl_buf_data_dynamic(uint32_t* buffer, const void* data, size_t size){
    glNamedBufferData(*buffer, size, data, GL_DYNAMIC_DRAW);
}

extern void tgl_buf_sub_data(uint32_t *buffer, const void *data, size_t size){
    glNamedBufferSubData(*buffer, 0, size, data);
}

extern void tgl_bind_arr_buf(uint32_t* vo){
    glBindVertexArray(*vo);
}

extern void tgl_bind_buf(uint32_t* vo){
    glBindBuffer(GL_ARRAY_BUFFER, *vo);
}

extern void tgl_buf_storage(int target, size_t size, void* data, GLbitfield flags){
    glBufferStorage(target, size, data, flags);
}

extern void* tgl_map_buffer(int target, size_t size, GLbitfield flags){
    return glMapBufferRange(target, 0, size, flags);
}

extern void tgl_arr_unbind(){
    glBindVertexArray(0);
}

void tgl_buf_unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//GL_ARRAY_BUFFER
//GL_ELEMENT_ARRAY_BUFFER
extern void tgl_bind_data_static(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
}

extern void tgl_bind_data_dynamic(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferData(target, size, data, GL_DYNAMIC_DRAW);
}

extern void tgl_bind_sub_data(uint32_t* buffer, const void* data, size_t size, int target){
    glBindBuffer(target, *buffer);
    glBufferSubData(target, 0, size, data);
}

extern void tgl_send_attrib(int layout, int size, int stride, int ptr){
    glVertexAttribPointer(
        layout, size,
        GL_FLOAT, GL_FALSE,
        stride * sizeof(float),
        (void*)(ptr * sizeof(float))
    );
    glEnableVertexAttribArray(layout);
}

extern void tgl_attrib_vertex_buffer(uint32_t *vao, uint32_t *vbo, int stride){
    glVertexArrayVertexBuffer(*vao, 0, *vbo, 0, stride * sizeof(float));
}

extern void tgl_attrib_arr_format(uint32_t *vao, int layout, int size, int offset){
    glVertexArrayAttribFormat(
        *vao, layout, size,
        GL_FLOAT, GL_FALSE,
        offset * sizeof(float)
    );
}

extern void tgl_enable_array_attrib(uint32_t *vao, int layout){
    glEnableVertexArrayAttrib(*vao, layout);
}

extern void tgl_bind_vertex_attrib(uint32_t *vao, int attrib, int layout){
    glVertexArrayAttribBinding(*vao, layout, attrib);
}

// std::vector<vertex> tgl_SetNDC(){
//     return {
//         {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
//         {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
//         {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//         {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
//     };
// }

extern void tgl_set_initial_vertex(vertex* data){
    data[0] = {{ 0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
    data[1] = {{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
    data[2] = {{-0.5f,-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
    data[3] = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
}

extern int getShaderError(uint32_t* shader){
    int pass;
    char log[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &pass);

    if (!pass){
        getShaderLogInfo(shader, log);
        return 1;
    }

    return 0;
}

extern int getShaderProgError(uint32_t* prog){
    int pass;
    char log[512];
    glGetProgramiv(*prog, GL_LINK_STATUS, &pass);

    if (!pass){
        getProgramLogInfo(prog, log);
        return 1;
    }

    return 0;
}

extern int tgl_create_shader(uint32_t* sh, const char* src, int target){
    *sh = glCreateShader(target);
    glShaderSource(*sh, 1, &src, NULL);
    glCompileShader(*sh);
    return getShaderError(sh);
}

extern int tgl_gen_shader_prog(uint32_t* prog, uint32_t* vs, uint32_t* fs){
    *prog = glCreateProgram();
    linkProgram(prog, *vs, *fs);
    return getShaderProgError(prog);
}

extern void tgl_delete_shaders(uint32_t vs, uint32_t fs){
    if (vs != 0) glDeleteShader(vs);
    if (fs != 0) glDeleteShader(fs);
}

extern void tgl_set_uniform(GLint loc, float* mat){
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

extern void tgl_clear_screen(color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern void tgl_del_prog(uint32_t* obj){
    if (obj != 0) glDeleteProgram(*obj);
}

extern void tgl_del_arr(uint32_t* obj){
    if (obj != 0) glDeleteVertexArrays(1, obj);
}

extern void tgl_del_buffer(uint32_t* obj){
    if (obj != 0) glDeleteBuffers(1, obj);
}

#endif

#ifdef __cplusplus
}
#endif
