// #define FGL_IMPL
#define BML_IMPL

#include "mkr.h"
#include "bml.h"
#include "val.h"
#include "fgl.h"
#include <stdint.h>
#include <stdio.h>

shader dshader;
texture dtex;
mesh dquad;
font dfont;
gbatch dbat;

bool mk_load_default_shader(){
    char* vsfile = fgl_load_shader_source("eng/simple.vert");
    char* fsfile = fgl_load_shader_source("eng/simple.frag");

    uint32_t vs = 0;
    uint32_t fs = 0;

    if (!fgl_create_shader(&vs, GL_VERTEX_SHADER)){
        printf("error: failed to create vertex shader\n");
        free(vsfile);
        vsfile = NULL;
        return false;
    }

    if (!fgl_compile_shader_source(vs, vsfile)){
        printf("error: failed to compile vertex shader\n");
        free(vsfile);
        vsfile = NULL;
        return false;
    }

    free(vsfile);
    vsfile = NULL;

    if (!fgl_create_shader(&fs, GL_FRAGMENT_SHADER)){
        printf("error: failed to create fragment shader\n");
        free(fsfile);
        fsfile = NULL;
        return false;
    }

    if (!fgl_compile_shader_source(fs, fsfile)){
        printf("error: failed to compile fragment shader\n");
        free(fsfile);
        fsfile = NULL;
        return false;
    }

    free(fsfile);
    fsfile = NULL;

    if (!fgl_create_shader_prog(&dshader.id)){
        printf("error: failed to create shader program\n");
        return false;
    }

    if (!fgl_attach_shader(dshader.id, vs, fs)){
        printf("error: failed to link shader program\n");
        return false;
    }

    fgl_uniform_location(dshader.id, &dshader.umodel, "uMvp");
    fgl_uniform_location(dshader.id, &dshader.utex, "uTexture");

    

    fgl_delete_shader(vs);
    fgl_delete_shader(fs);

    return true;
}

void mk_unload_default_shader(){
    fgl_delete_prog(dshader.id);
}

bool mk_init(){
    if (mk_load_default_shader()){
        printf("\t[INFO] DEFAULT SHADER - OK\n");
    }

    if (mk_load_default_texture()){
        printf("\t[INFO] DEFAULT TEXTURE - OK\n");
    }

    if (mk_load_default_quad()){
        printf("\t[INFO] DEFAULT SHAPES - OK\n");
    }

    if (mk_load_default_font()){
        printf("\t[INFO] DEFAULT FONT - OK\n");
    }

    printf("\t[INFO] SOLVING INTERNALS...\n");
    mk_load_gbatch();

    printf("\t[INFO] DEFAULT STATE - OK\n");
    return true;
}

void mk_shutdown(){
    mk_unload_gbatch();
    mk_unload_default_font();
    mk_unload_default_quad();
    mk_unload_default_texture();
    mk_unload_default_shader();

    printf("\t[INFO] DEFAULT STATE UNLOADED\n");
}

void mk_std_vertex_size(vec2 position, vec2 size, color color, float u0, float v0, float u1, float v1){
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
}

void mk_push_vertex_position(vec3 position){
    val_arenav_add_position(&dbat.verts, position.x, position.y, position.z);
}

void mk_push_vertex_color(color color){
    val_arenav_add_color(&dbat.verts, color.r, color.g, color.b);
}

void mk_push_vertex_uv(float u, float v){
    val_arenav_add_uv(&dbat.verts, u, v);
}

void mk_push_indices(uint32_t base){
    val_arenai_add_idx(&dbat.indices, base + 0);
    val_arenai_add_idx(&dbat.indices, base + 1);
    val_arenai_add_idx(&dbat.indices, base + 3);

    val_arenai_add_idx(&dbat.indices, base + 1);
    val_arenai_add_idx(&dbat.indices, base + 2);
    val_arenai_add_idx(&dbat.indices, base + 3);
}

void mk_push_call(uint32_t count, uint32_t start, uint32_t *tex){
    val_arenad_add_call(&dbat.calls, count, start, tex);
}

void flush_limit(){
    if (dbat.verts.count >= VMAX || dbat.indices.count >= IMAX){
        flush();
    }
}

void flush(){
    if (val_arenav_empty(&dbat.verts)){
        return;
    }

    fgl_sub_data(
        &dbat.buffer,
        sizeof(vertex) * dbat.verts.count,
        sizeof(uint32_t) * dbat.indices.count,
        dbat.verts.data, dbat.indices.data
    );

    fgl_bind_arr(&dbat.buffer);

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

void mk_render_begin(){
    fgl_uniform_prog(&dshader.id, &dshader.umodel);
    fgl_uniform_prog(&dshader.id, &dshader.utex);
    fgl_prog_use(dshader.id);
}

void mk_render_end(){
    flush();
    matrix view = bml_mat_identity();
    fgl_uniform_mat(&dshader.id, &dshader.umodel, &view.m[0][0]);
}

void mk_ortho_start(camera_data_2d cam_data, int width, int height){
    matrix proj = bml_mat_ortho(
        0.0f, width,
        0.0f, height,
        1000.0, 0.0f
    );

    vec3 cam_position = {
        cam_data.position.x,
        cam_data.position.y,
        0.0f
    };

    matrix view = bml_mat_view(
        cam_position,
        cam_data.rotation,
        cam_data.zoom
    );

    matrix model = bml_mat_identity();

    matrix mvp = bml_mat_mul(bml_mat_mul(proj, view), model);
    fgl_uniform_mat(&dshader.id, &dshader.umodel, &mvp.m[0][0]);
}
