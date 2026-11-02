#define FGL_IMPL
#include "mkr.h"
#include "fgl.h"
#include <stdint.h>
#include <stdio.h>

shader dshader;

extern bool mk_load_default_shader(){
    char* vsfile = fgl_load_shader_source("eng/simple.vert");
    char* fsfile = fgl_load_shader_source("eng/simple.frag");

    uint32_t vs = 0;
    uint32_t fs = 0;

    fgl_create_shader(&vs, GL_VERTEX_SHADER);
    fgl_compile_shader_source(vs, vsfile);
    free(vsfile);
    vsfile = NULL;

    fgl_create_shader(&fs, GL_FRAGMENT_SHADER);
    fgl_compile_shader_source(fs, fsfile);
    free(fsfile);
    fsfile = NULL;

    fgl_create_shader_prog(&dshader.id);
    fgl_attach_shader(dshader.id, vs, fs);

    fgl_uniform_location(dshader.id, &dshader.umodel, "uMvp");
    fgl_uniform_location(dshader.id, &dshader.utex, "uTexture");

    fgl_uniform_prog(&dshader.id, &dshader.umodel);
    fgl_uniform_prog(&dshader.id, &dshader.utex);

    fgl_delete_shader(vs);
    fgl_delete_shader(fs);

    return true;
}

void mk_unload_default_shader(){
    if (dshader.id != 0){
        fgl_delete_prog(dshader.id);
    }
}


bool mk_init(){
    mk_load_default_shader();

    printf("[INFO] DEFAULT STATE - OK\n");
    return true;
}

void mk_shutdown(){
    mk_unload_default_shader();
}
