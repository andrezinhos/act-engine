#define FGL_IMPL
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
        free(vsfile);
        vsfile = NULL;
        return false;
    }

    if (!fgl_compile_shader_source(vs, vsfile)){
        fgl_get_shader_log(vs);
        free(vsfile);
        vsfile = NULL;
        return false;
    }

    free(vsfile);
    vsfile = NULL;

    if (!fgl_create_shader(&fs, GL_FRAGMENT_SHADER)){
        free(fsfile);
        fsfile = NULL;
        return false;
    }

    if (!fgl_compile_shader_source(fs, fsfile)){
        fgl_get_shader_log(fs);
        free(fsfile);
        fsfile = NULL;
        return false;
    }

    free(fsfile);
    fsfile = NULL;

    if (!fgl_create_shader_prog(&dshader.id)){
        return false;
    }

    if (!fgl_link_shader_prog(dshader.id, vs, fs)){
        return false;
    }

    fgl_uniform_location(dshader.id, &dshader.umodel, "uMvp");
    fgl_uniform_location(dshader.id, &dshader.utex, "uTexture");

    fgl_uniform_prog(&dshader.id, &dshader.utex);

    fgl_delete_shader(vs);
    fgl_delete_shader(fs);

    return true;
}

void mk_unload_default_shader(){
    fgl_delete_prog(dshader.id);
}

bool mk_init(){
    if (mk_load_default_shader()){
        printf("[INFO] DEFAULT SHADER - OK\n");
    }

    if (mk_load_default_texture()){
        printf("[INFO] DEFAULT TEXTURE - OK\n");
    }

    if (mk_load_default_quad()){
        printf("[INFO] DEFAULT SHAPES - OK\n");
    }

    if (mk_load_default_font()){
        printf("[INFO] DEFAULT FONT - OK\n");
    }

    printf("[INFO] SOLVING INTERNALS...\n");
    mk_load_gbatch();

    fgl_enable_blend(true);

    printf("[INFO] DEFAULT STATE - OK\n");
    return true;
}

void mk_shutdown(){
    mk_unload_gbatch();
    mk_unload_default_font();
    mk_unload_default_quad();
    mk_unload_default_texture();
    mk_unload_default_shader();

    printf("[INFO] DEFAULT STATE UNLOADED\n");
}

void mk_clear_screen(color color){
    fgl_clear_screen(color);
}

void mk_render_begin(){
    fgl_prog_use(dshader.id);
}

void mk_render_end(){
    mk_flush();
    matrix view = bml_mat_identity();
    fgl_uniform_mat(&dshader.id, &dshader.umodel, &view.m[0][0]);
}

void mk_ortho_start(camera_data_2d cam_data, int width, int height){
    matrix proj = bml_mat_ortho(
        0.0f, width,
        0.0f, height,
        -1.0, 1.0f
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
