/*
    IN TESTING PROGRESS
*/

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "fgl.h"
#include "tgt.h"
#include "val.h"
#include "tfb.h"
#include "bml.h"

#define VMAX 1024
#define IMAX 1536
#define CMAX 512

typedef struct font{
    uint8_t* data;
    glyph pack[96];
    tfb_base base;
    float spacing;
    texture ftex;
} font;

typedef struct mesh {
    fgl_buffer buffer;
    arena_v verts;
    arena_i indices;
} mesh;

typedef struct {
    arena_v verts;
    arena_i indices;
    arena_dc calls;
    fgl_buffer buffer;
} gbatch;

extern shader dshader;
extern texture dtex;
extern mesh dquad;
extern font dfont;
extern gbatch dbat;

#ifdef __cplusplus
extern "C" {
#endif

extern bool mk_load_default_shader();
extern void mk_unload_default_shader();

extern bool mk_load_default_texture();
extern void mk_unload_default_texture();

extern bool mk_load_default_quad();
extern void mk_unload_default_quad();

extern bool mk_load_default_font();
extern void mk_unload_default_font();

extern bool mk_load_gbatch();
extern void mk_unload_gbatch();

extern bool mk_init();
extern void mk_shutdown();

extern texture mk_load_tex_memory(void* path, int width, int height);
extern void mk_delete_tex(texture* tex);

extern void mk_render_tex_region(texture* tex, rectangle source, vec2 position, vec2 size, color color);
extern void mk_render_tex();

extern mesh mk_gen_mesh(size_t verts, size_t indices);
extern void mk_push_mesh(mesh* mesh, bool dynamic);

extern void mk_delete_mesh(mesh* mesh);

extern void mk_push_vertex_position(vec3 position);
extern void mk_push_vertex_color(color color);
extern void mk_push_vertex_uv(float u, float v);

extern void mk_push_call(uint32_t count, uint32_t start, uint32_t* tex);

extern void mk_push_indices(uint32_t base);

extern void flush_limit();
extern void flush();

#ifdef __cplusplus
}
#endif
