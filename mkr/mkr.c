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

    fgl_uniform_prog(&dshader.id, &dshader.umodel);
    fgl_uniform_prog(&dshader.id, &dshader.utex);

    fgl_delete_shader(vs);
    fgl_delete_shader(fs);

    return true;
}

void mk_unload_default_shader(){
    fgl_delete_prog(dshader.id);
}

bool mk_init(){

    printf("\t[INFO] DEFAULT SHADER LOADING...\n");
    mk_load_default_shader();
    printf("\t[INFO] DEFAULT TEXTURE LOADING...\n");
    mk_load_default_texture();
    printf("\t[INFO] DEFAULT SHAPES LOADING...\n");
    mk_load_default_quad();
    printf("\t[INFO] DEFAULT FONT LOADING...\n");
    mk_load_default_font();
    printf("\t[INFO] SOLVING INTERNALS...\n");
    mk_load_gbatch();

    printf("[INFO] DEFAULT STATE - OK\n");
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
    if (dbat.verts.count == 0) return;

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

        //TODO: add draw by calls
        fgl_draw_elements(d->count, (void*)(d->start * sizeof(uint32_t)));
    }

    fgl_unbind_arr();

    val_arenav_reset(&dbat.verts);
    val_arenai_reset(&dbat.indices);
    val_arenad_reset(&dbat.calls);
}
