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

typedef struct camera_data_2d{
    vec2 position;
    float rotation;
    float zoom;
} camera_data_2d;

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

extern void mk_std_vertex_size(vec2 position, vec2 size, color color, float u0, float v0, float u1, float v1);

extern texture mk_load_tex_memory(void* data, int width, int height);
extern void mk_delete_tex(texture* tex);

extern void mk_render_tex_region(texture* tex, rectangle source, vec2 position, vec2 size, color color);
extern void mk_render_tex(texture* tex, vec2 position, vec2 size);
extern void mk_render_rectangle(vec2 position, vec2 size, color color);

extern mesh mk_gen_mesh_data(size_t verts, size_t indices);
extern void mk_push_mesh(mesh* mesh, bool dynamic);
extern mesh mk_gen_quad(int width, int height);

extern void mk_delete_mesh(mesh* mesh);

extern void mk_push_vertex_position(vec3 position);
extern void mk_push_vertex_color(color color);
extern void mk_push_vertex_uv(float u, float v);

extern void mk_push_call(uint32_t count, uint32_t start, uint32_t* tex);

extern void mk_push_indices(uint32_t base);

extern void mk_clear_screen(color color);

extern void flush_limit();
extern void flush();

extern void mk_render_begin();
extern void mk_render_end();
extern void mk_ortho_start(camera_data_2d cam_data, int width, int height);

#ifdef __cplusplus
}
#endif
