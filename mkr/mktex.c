#include "mkr.h"
#include "fgl.h"
#include "val.h"
#include <stdint.h>

bool mk_load_default_texture(){
    dtex.width = 1;
    dtex.height = 1;

    uint32_t color = 0xFFFFFFFF;
    fgl_create_texture(&dtex.id, GL_TEXTURE_2D, dtex.width, dtex.height, &color);

    if (dtex.id == 0){
        printf("error: failed to create default texture\n");
        return false;
    }

    fgl_texture_repeat(dtex.id);
    fgl_texture_filter(dtex.id, GL_NEAREST);

    return true;
}

void mk_unload_default_texture(){
    fgl_delete_texture(&dtex.id);
}

texture mk_load_tex_memory(void* data, int width, int height){
    texture tex;
    tex.width = width;
    tex.height = height;

    fgl_create_texture(&tex.id, GL_TEXTURE_2D, width, height, data);

    fgl_texture_repeat(tex.id);
    fgl_texture_filter(tex.id, GL_NEAREST);

    return tex;
}

void mk_delete_tex(texture* tex){
    fgl_delete_texture(&tex->id);
}

void mk_render_tex_region(texture* tex, rectangle source, vec2 position, vec2 size, color color){
    uint32_t index_start = val_arenav_size(&dbat.verts);
    uint32_t base = val_arenai_size(&dbat.indices);

    float u0 = source.x / (float)tex->width;
    float u1 = (source.x + source.width) / (float)tex->width;

    float v0 = source.y / (float)tex->height;
    float v1 = (source.y + source.height) / (float)tex->height;

    mk_push_vertex_position((vec3){position.x, position.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(u0, v0);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x + size.x, position.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(u1, v0);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x + size.x, position.y + size.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(u1, v1);

    val_arenav_inc_count(&dbat.verts);

    mk_push_vertex_position((vec3){position.x, position.y + size.y, 0.0f});
    mk_push_vertex_color(color);
    mk_push_vertex_uv(u0, v1);

    val_arenav_inc_count(&dbat.verts);

    mk_push_indices(base);

    if (val_arenad_empty(&dbat.calls) || val_arenad_back_tex(&dbat.calls) != &tex->id){
        mk_push_call(index_start, 6, &tex->id);
    }
    else {
        val_arenad_add_count(&dbat.calls, 6);
    }
}
