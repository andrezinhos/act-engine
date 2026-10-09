#pragma once
#include "glad.h"
#include "mkmath.hpp"
#include "mktypes.h"
#include <cstddef>
#include <vector>

#define MKR_POSITION_LAYOUT 0
#define MKR_COLOR_LAYOUT 1
#define MKR_TEXTURE_LAYOUT 2
#define MKR_VERTEX_STRIDE 8

#define VMAX 1004
#define IMAX 1506
#define CMAX 512

typedef struct {
    uint start;
    uint count;
    Texture* texref;
} DCall;

typedef struct {
    vertex* verts;
    int count;
    int cap;
} ArenaV;

typedef struct {
    uint inds;
    int count;
    int cap;
} ArenaI;

typedef struct {
    DCall* dc;
    int count;
    int cap;
} ArenaDC;

typedef struct {
    std::vector<vertex> vertices;
    std::vector<uint> indices;
    std::vector<DCall> calls;
    uint vao, vbo, ebo;
} Batch;

namespace mkgl{
    byte* loadBytes(cstr path, size_t* size);
    void enable_blend(bool flag);
    void get_error(); // is useless actually

    bool gen_array(uint* obj);
    bool gen_buffer(uint* obj);

    bool create_buffer(uint* vo);
    bool create_array(uint* v);

    void element_connect(uint* vao, uint* vo);

    void buf_data_static(uint* buffer, const void* data, size_t size);
    void buf_data_dynamic(uint* buffer, const void* data, size_t size);
    void buf_sub_data(uint* buffer, const void* data, size_t size);
    void buf_storage(int target, size_t size, void* data, GLbitfield flags);
    void* map_buffer(int target, size_t size, GLbitfield flags);

    void arr_unbind();
    void buf_unbind();

    void bind_arr_buf(uint* vo);
    void bind_buf(uint* vo);
    void bind_data_static(uint* buffer, const void* data, size_t size, int target);
    void bind_data_dynamic(uint* buffer, const void* data, size_t size, int target);
    void bind_sub_data(uint* buffer, const void* data, size_t size, int target);

    // void sendMapAttribPtr(int layout, int locSize, int stride, int ptr);
    void sendAttribPtr(int layout, int locSize, int stride, int ptr);

    void set_attrib_vertex_buffer(uint* vao, uint* vbo, int stride);
    void set_attrib_format(uint *vao, int layout, int size, int offset);
    void enable_array_attrib(uint* vao, int layout);
    void bind_vertex_attrib(uint* vao, int attrib, int layout);

    char* loadShaderFile(cstr path);
    void set_initial_vertex(vertex* data);

    bool create_shader(uint* sh, cstr src, int target);
    bool gen_shader_prog(uint* prog, uint vs, uint fs);
    void delete_shaders(uint vs, uint fs);
    void setUniformMat(GLint loc, Matrix* mat);
    void clearScreen(Color color);

    void del_arr(uint* obj);
    void del_buffer(uint* obj);
    void del_prog(uint* obj);
};
