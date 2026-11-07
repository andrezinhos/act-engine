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

bool mk_load_gbatch(){
    val_arenav_init(&dbat.verts, VMAX);
    val_arenai_init(&dbat.indices, IMAX);
    val_arenad_init(&dbat.calls, CMAX);

    fgl_create_array(&dquad.buffer);
    fgl_create_buffer(&dquad.buffer);

    fgl_data_dynamic(
        &dquad.buffer,
        sizeof(vertex) * VMAX,
        sizeof(uint32_t) * IMAX,
        NULL, NULL
    );

    fgl_element_connect(&dquad.buffer);
    fgl_buffer_connect(&dquad.buffer);

    fgl_arr_attrib(&dquad.buffer, FGL_POSITION_VALUE, FGL_POSITION_LAYOUT, 0);
    fgl_arr_attrib(&dquad.buffer, FGL_COLOR_VALUE, FGL_COLOR_LAYOUT, 3);
    fgl_arr_attrib(&dquad.buffer, FGL_TEXTURE_VALUE, FGL_TEXTURE_LAYOUT, 6);

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

mesh mk_gen_mesh_data(size_t verts, size_t indices){
    mesh m;

    val_arenav_init(&m.verts, verts);
    val_arenai_init(&m.indices, indices);

    m.verts.count = verts;
    m.indices.count = indices;

    fgl_create_array(&m.buffer);
    fgl_create_buffer(&m.buffer);

    return m;
}

void mk_push_mesh(mesh* mesh, bool dynamic){

    if (dynamic){
        fgl_data_dynamic(&mesh->buffer,
            vertex_size(mesh->verts.count),
            element_size(mesh->indices.count),
            mesh->verts.data, mesh->indices.data
        );
    }
    else {
        fgl_data_static(&mesh->buffer,
            vertex_size(mesh->verts.count),
            element_size(mesh->indices.count),
            mesh->verts.data, mesh->indices.data
        );
    }

    fgl_element_connect(&mesh->buffer);
    fgl_buffer_connect(&mesh->buffer);

    fgl_arr_attrib(&mesh->buffer, FGL_POSITION_VALUE, FGL_POSITION_LAYOUT, 0);
    fgl_arr_attrib(&mesh->buffer, FGL_COLOR_VALUE, FGL_COLOR_LAYOUT, 3);
    fgl_arr_attrib(&mesh->buffer, FGL_TEXTURE_VALUE, FGL_TEXTURE_LAYOUT, 6);
}

void mk_delete_mesh(mesh* mesh){
    val_arenav_free(&mesh->verts);
    val_arenai_free(&mesh->indices);
    fgl_delete_buffer(&mesh->buffer.ebo);
    fgl_delete_buffer(&mesh->buffer.vbo);
    fgl_delete_arr(&mesh->buffer.vao);
}

void mk_render_rectangle(vec2 position, vec2 size, color color){
    flush_limit();

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

    mk_push_indices(base);

    if (val_arenad_empty(&dbat.calls) || val_arenad_back_tex(&dbat.calls) != &dtex.id){
        mk_push_call(index_start, 6, &dtex.id);
    }
    else {
        val_arenad_add_count(&dbat.calls, 6);
    }
}
