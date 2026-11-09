#include "mkr.h"

void mk_arena_push_position(arena_v* arena, vec3 position){
    val_arenav_add_position(arena, position.x, position.y, position.z);
}

void mk_arena_push_color(arena_v* arena, color color){
    val_arenav_add_color(arena, color.r, color.g, color.b);
}

void mk_arena_push_uv(arena_v* arena, float u, float v){
    val_arenav_add_uv(arena, u, v);
}

void mk_push_vertex_position(vec3 position){
    mk_arena_push_position(&dbat.verts, position);
}

void mk_push_vertex_color(color color){
    mk_arena_push_color(&dbat.verts, color);
}

void mk_push_vertex_uv(float u, float v){
    mk_arena_push_uv(&dbat.verts, u, v);
}

void mk_push_indices(uint32_t base){
    val_arenai_add_idx(&dbat.indices, base + 0);
    val_arenai_add_idx(&dbat.indices, base + 1);
    val_arenai_add_idx(&dbat.indices, base + 3);

    val_arenai_add_idx(&dbat.indices, base + 1);
    val_arenai_add_idx(&dbat.indices, base + 2);
    val_arenai_add_idx(&dbat.indices, base + 3);
}

void mk_push_call(uint32_t start, uint32_t count, uint32_t *tex){
    val_arenad_add_call(&dbat.calls, count, start, tex);
}

mesh mk_gen_mesh_data(size_t verts, size_t indices){
    mesh m;

    val_arenav_init(&m.verts, verts);
    val_arenai_init(&m.indices, indices);

    fgl_create_array(&m.buffer.vao);
    fgl_create_buffer(&m.buffer.vbo, &m.buffer.ebo);

    return m;
}

void mk_push_mesh(mesh* mesh, bool dynamic){

    if (dynamic){
        fgl_data_dynamic(
            &mesh->buffer.vbo,
            vertex_size(mesh->verts.count),
            mesh->verts.data
        );

        fgl_data_dynamic(
            &mesh->buffer.ebo,
            element_size(mesh->indices.count),
            mesh->indices.data
        );
    }
    else {
        fgl_data_static(
            &mesh->buffer.vbo,
            vertex_size(mesh->verts.count),
            mesh->verts.data
        );

        fgl_data_static(
            &mesh->buffer.ebo,
            element_size(mesh->indices.count),
            mesh->indices.data
        );
    }

    fgl_element_connect(mesh->buffer.vao, mesh->buffer.ebo);
    fgl_buffer_connect(mesh->buffer.vao, mesh->buffer.vbo, 8 * sizeof(float));

    fgl_arr_attrib(mesh->buffer.vao, FGL_POSITION_LAYOUT, FGL_POSITION_VALUE, 0);
    fgl_arr_attrib(mesh->buffer.vao, FGL_COLOR_LAYOUT, FGL_COLOR_VALUE, 3);
    fgl_arr_attrib(mesh->buffer.vao, FGL_TEXTURE_LAYOUT, FGL_TEXTURE_VALUE, 6);
}

void mk_delete_mesh(mesh* mesh){
    val_arenav_free(&mesh->verts);
    val_arenai_free(&mesh->indices);
    fgl_delete_buffer(&mesh->buffer.ebo);
    fgl_delete_buffer(&mesh->buffer.vbo);
    fgl_delete_arr(&mesh->buffer.vao);
}

void mk_flush_limit(){
    if (dbat.verts.count + 4 >= VMAX ||
        dbat.indices.count + 6 >= IMAX)
    {
        mk_flush();
    }
}

void mk_flush(){
    if (val_arenad_empty(&dbat.calls) || val_arenav_empty(&dbat.verts)){
        return;
    }

    fgl_sub_data(
        &dbat.buffer.vbo,
        sizeof(vertex) * dbat.verts.count,
        dbat.verts.data
    );

    fgl_sub_data(
        &dbat.buffer.ebo,
        sizeof(uint32_t) * dbat.indices.count,
        dbat.indices.data
    );

    fgl_bind_arr(dbat.buffer.vao);

    for (size_t i = 0; i < dbat.calls.count; i++){
        dcall* d = &dbat.calls.data[i];

        fgl_texture_bind(0, *d->texid);

        fgl_draw_elements(
            d->count,
            (void*)(d->start * sizeof(uint32_t))
        );
    }

    fgl_unbind_arr();

    val_arenav_reset(&dbat.verts);
    val_arenai_reset(&dbat.indices);
    val_arenad_reset(&dbat.calls);
}
