#pragma once
#include "glad.h"
#include "mkmath.hpp"
#include "mktypes.h"
#include <stdint.h>

#define MKR_POSITION_LAYOUT 0
#define MKR_COLOR_LAYOUT 1
#define MKR_TEXTURE_LAYOUT 2
#define MKR_VERTEX_STRIDE 8

#define VMAX 1004
#define IMAX 1506
#define CMAX 512

namespace mkgl{
    void enable_blend(bool flag);
    void get_error(); // is useless actually

    bool gen_array(uint32_t* obj);
    bool gen_buffer(uint32_t* obj);

    bool create_buffer(uint32_t* vo);
    bool create_array(uint32_t* v);

    void element_connect(uint32_t* vao, uint32_t* vo);

    void buf_data_static(uint32_t* buffer, const void* data, size_t size);
    void buf_data_dynamic(uint32_t* buffer, const void* data, size_t size);
    void buf_sub_data(uint32_t* buffer, const void* data, size_t size);
    void buf_storage(int target, size_t size, void* data, GLbitfield flags);
    void* map_buffer(int target, size_t size, GLbitfield flags);

    void arr_unbind();
    void buf_unbind();

    void bind_arr_buf(uint32_t* vo);
    void bind_buf(uint32_t* vo);
    void bind_data_static(uint32_t* buffer, const void* data, size_t size, int target);
    void bind_data_dynamic(uint32_t* buffer, const void* data, size_t size, int target);
    void bind_sub_data(uint32_t* buffer, const void* data, size_t size, int target);

    // void sendMapAttribPtr(int layout, int locSize, int stride, int ptr);
    void sendAttribPtr(int layout, int locSize, int stride, int ptr);

    void set_attrib_vertex_buffer(uint32_t* vao, uint32_t* vbo, int stride);
    void set_attrib_format(uint32_t *vao, int layout, int size, int offset);
    void enable_array_attrib(uint32_t* vao, int layout);
    void bind_vertex_attrib(uint32_t* vao, int attrib, int layout);

    char* loadShaderFile(const char* path);
    // void set_initial_vertex(vertex* data);

    bool create_shader(uint32_t* sh, const char* src, int target);
    bool gen_shader_prog(uint32_t* prog, uint32_t vs, uint32_t fs);
    void delete_shaders(uint32_t vs, uint32_t fs);
    void setUniformMat(int loc, Matrix* mat);
    // void clearScreen(Color color);

    void del_arr(uint32_t* obj);
    void del_buffer(uint32_t* obj);
    void del_prog(uint32_t* obj);
};
