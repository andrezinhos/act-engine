#define VAL_IMPL

#include "fgl.h"
#include "mkr.h"
#include "val.h"
#include "tgt.h"

bool mk_load_default_quad(){
    // val_arenav_init(&dquad.verts, 4);
    // val_arenai_init(&dquad.indices, 6);

    // dquad.verts.count = 4;
    // dquad.indices.count = 6;

    // fgl_create_array(&dquad.buffer);
    // fgl_create_buffer(&dquad.buffer);

    dquad = mk_gen_mesh_data(4, 6);
    mk_push_mesh(&dquad, false);

    // fgl_data_static(
        // &dquad.buffer,
        // sizeof(vertex) * dquad.verts.count,
        // sizeof(uint32_t) * dquad.indices.count,
        // dquad.verts.data, dquad.indices.data
    // );

    // fgl_element_connect(&dquad.buffer);
    // fgl_buffer_connect(&dquad.buffer);

    // fgl_arr_attrib(&dquad.buffer, FGL_POSITION_VALUE, FGL_POSITION_LAYOUT, 0);
    // fgl_arr_attrib(&dquad.buffer, FGL_COLOR_VALUE, FGL_COLOR_LAYOUT, 3);
    // fgl_arr_attrib(&dquad.buffer, FGL_TEXTURE_VALUE, FGL_TEXTURE_LAYOUT, 6);

    return true;
}

void mk_unload_default_quad(){
    val_arenav_free(&dquad.verts);
    val_arenai_free(&dquad.indices);
    fgl_delete_buffer(&dquad.buffer.ebo);
    fgl_delete_buffer(&dquad.buffer.vbo);
    fgl_delete_arr(&dquad.buffer.vao);
}

mesh mk_gen_quad(int width, int height, int x, int y, color color){
    mesh m = {};

    m = mk_gen_mesh_data(4, 6);
    mk_push_mesh(&m, false);

    m.indices.count = 6;
    m.verts.count = 4;

    uint32_t base = m.indices.count;

    mk_arena_push_position(&m.verts, (vec3){x, y, 0.0f});
    mk_arena_push_color(&m.verts, color);
    mk_arena_push_uv(&m.verts, 0.0f, 0.0f);

    val_arenav_inc_count(&m.verts);

    mk_arena_push_position(&m.verts, (vec3){x + width, y, 0.0f});
    mk_arena_push_color(&m.verts, color);
    mk_arena_push_uv(&m.verts, 1.0f, 0.0f);

    val_arenav_inc_count(&m.verts);

    mk_arena_push_position(&m.verts, (vec3){x + width, y + height, 0.0f});
    mk_arena_push_color(&m.verts, color);
    mk_arena_push_uv(&m.verts, 1.0f, 1.0f);

    val_arenav_inc_count(&m.verts);

    mk_arena_push_position(&m.verts, (vec3){x, y + height, 0.0f});
    mk_arena_push_color(&m.verts, color);
    mk_arena_push_uv(&m.verts, 0.0f, 1.0f);

    // val_arenav_inc_count(&m.verts);

    mk_push_indices(base);

    return m;
}

bool mk_load_gbatch(){
    if (!val_arenav_init(&dbat.verts, VMAX)){
        printf("error: failed to init batch: vertex arena fail\n");
        return false;
    }

    if (!val_arenai_init(&dbat.indices, IMAX)){
        printf("error: failed to init batch: index arena fail\n");
        return false;
    }

    if (!val_arenad_init(&dbat.calls, CMAX)){
        printf("error: failed to init batch: draw call arena fail\n");
        return false;
    }

    if (!fgl_create_array(&dbat.buffer.vao)){
        printf("error: failed to init batch: vao fail\n");
        return false;
    }

    if (!fgl_create_buffer(&dbat.buffer.vbo, &dbat.buffer.ebo)){
        printf("error: failed to init batch: vbo/ebo fail\n");
        return false;
    }

    fgl_data_dynamic(
        &dbat.buffer.vbo,
        sizeof(vertex) * VMAX,
        NULL
    );

    fgl_data_dynamic(
        &dbat.buffer.ebo,
        sizeof(uint32_t) * IMAX,
        NULL
    );

    fgl_element_connect(&dbat.buffer);
    fgl_buffer_connect(&dbat.buffer);

    fgl_arr_attrib(dbat.buffer.vao, FGL_POSITION_LAYOUT, FGL_POSITION_VALUE, 0);
    fgl_arr_attrib(dbat.buffer.vao, FGL_COLOR_LAYOUT, FGL_COLOR_VALUE, 3);
    fgl_arr_attrib(dbat.buffer.vao, FGL_TEXTURE_LAYOUT, FGL_TEXTURE_VALUE, 6);

    return true;
}

void mk_unload_gbatch(){
    val_arenav_free(&dbat.verts);
    val_arenai_free(&dbat.indices);
    val_arenad_free(&dbat.calls);
    fgl_delete_buffer(&dbat.buffer.ebo);
    fgl_delete_buffer(&dbat.buffer.vbo);
    fgl_delete_arr(&dbat.buffer.vao);
}

void mk_render_rectangle(vec2 position, vec2 size, color color){
    mk_flush_limit();

    uint32_t index_start = val_arenav_size(&dbat.verts);
    uint32_t base = val_arenai_size(&dbat.indices);

    mk_push_vertex_position((vec3){position.x, position.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(0.0f, 0.0f);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x + size.x, position.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(1.0f, 0.0f);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x + size.x, position.y + size.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(1.0f, 1.0f);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x, position.y + size.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(0.0f, 1.0f);

    mk_push_indices(index_start);

    if (val_arenad_empty(&dbat.calls) || val_arenad_back_tex(&dbat.calls) != &dtex.id){
        mk_push_call(base, 6, &dtex.id);
    }
    else {
        val_arenad_add_count(&dbat.calls, 6);
    }
}
