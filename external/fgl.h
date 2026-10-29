#pragma once
#include "glad.h"
#include "tgt.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct fgl_buffer{
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
} fgl_buffer;

#define FGL_POSITION_LAYOUT 0
#define FGL_COLOR_LAYOUT 1
#define FGL_TEXTURE_LAYOUT 2

#define FGL_POSITION_VALUE 3
#define FGL_COLOR_VALUE 3
#define FGL_TEXTURE_VALUE 2

#ifdef FGL_STATIC
    #define FGLDEF static
#else
    #define FGLDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

FGLDEF size_t buffer_size(size_t data);
FGLDEF size_t element_size(size_t data);

FGLDEF void fgl_enable_blend(bool flag);

FGLDEF void fgl_clear_screen(color color);

FGLDEF void fgl_hardware_info();
FGLDEF int fgl_create_array(fgl_buffer* _buf);
FGLDEF int fgl_create_buffer(fgl_buffer* _buf);

FGLDEF void fgl_element_connect(fgl_buffer* _buf);
FGLDEF void fgl_buffer_connect(fgl_buffer* _buf);

FGLDEF void fgl_bind_arr(fgl_buffer* _buf);
FGLDEF void fgl_unbind_arr();

FGLDEF int fgl_data_static(fgl_buffer* _buf, size_t size, const void* data);
FGLDEF int fgl_data_dynamic(fgl_buffer* _buf, size_t size, const void* data);
FGLDEF int fgl_sub_data(fgl_buffer* _buf, size_t size, const void* data);

FGLDEF void fgl_arr_attrib(fgl_buffer* _buf, int size, int layout, int offset);

FGLDEF int fgl_create_shader(uint32_t* shader, int target);
FGLDEF char* fgl_load_shader_source(const char* s_path);
FGLDEF int fgl_compile_shader_source(uint32_t shader, const char* s_path);
FGLDEF int fgl_create_shader_prog(uint32_t* shader);
FGLDEF int fgl_attach_shader(uint32_t prog, uint32_t vs, uint32_t fs);

FGLDEF void fgl_uniform_location(uint32_t shader, uint32_t* uniform, const char* loc);
FGLDEF void fgl_uniform_mat(uint32_t* shader, uint32_t* loc, const float* value);
FGLDEF void fgl_use_prog(uint32_t* prog, uint32_t* uniform);

FGLDEF int fgl_create_texture(uint32_t* id, int target, int width, int height, void* pixels);
FGLDEF void fgl_texture_filter(uint32_t id, int filter);
FGLDEF void fgl_texture_repeat(uint32_t id);

FGLDEF void fgl_texture_bind(int unit, uint32_t id);

FGLDEF void fgl_draw_elements(size_t count, void* offset);

FGLDEF void fgl_delete_buffer(uint32_t* buffer);
FGLDEF void fgl_delete_arr(uint32_t* arr);
FGLDEF void fgl_delete_prog(uint32_t prog);
FGLDEF void fgl_delete_shader(uint32_t shader);
FGLDEF void fgl_delete_texture(uint32_t* texture);

#ifdef FGL_IMPL

static inline char* load_file(const char* path){
    FILE* file = fopen(path, "rb");

    if (!file){
        printf("error: shader file do not exist\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fsize + 1);

    if (!buffer){
        printf("error: couldn't create shader file bytes\n");
        return NULL;
    }

    size_t read = fread(buffer, 1, fsize, file);
    buffer[fsize] = '\0';
    fclose(file);

    if (read != fsize){
        printf("error: couldn't read file bytes\n");
        free(buffer);
        return NULL;
    }

    return buffer;
}

FGLDEF size_t buffer_size(size_t data){
    return sizeof(vertex) * data;
}
FGLDEF size_t element_size(size_t data){
    return sizeof(uint32_t) * data;
}

FGLDEF void fgl_enable_blend(bool flag){
    if (flag){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

FGLDEF void fgl_clear_screen(color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

FGLDEF void fgl_hardware_info(){
    const uint8_t* vendor = glGetString(GL_VENDOR);
    const uint8_t* render = glGetString(GL_RENDERER);
    const uint8_t* glver = glGetString(GL_VERSION);

    printf("[INFO] GPU: %s\n", render);
    printf("[INFO] VENDOR: %s\n", vendor);
    printf("[INFO] GL VERSION: %s\n", glver);
}

FGLDEF int fgl_create_array(fgl_buffer* _buf){
    glCreateBuffers(1, &_buf->vao);

    if (_buf->vao == 0){
        printf("error: vertex array failed\n");
        return 1;
    }

    return 0;
}

FGLDEF int fgl_create_buffer(fgl_buffer* _buf){
    glCreateBuffers(1, &_buf->vbo);
    glCreateBuffers(1, &_buf->ebo);

    if (_buf->vbo == 0){
        printf("error: vertex buffer failed\n");
        return 1;
    }
    if (_buf->ebo == 0){
        printf("error: vertex element buffer failed\n");
        return 1;
    }

    return 0;
}

FGLDEF void fgl_element_connect(fgl_buffer* _buf){
    glVertexArrayElementBuffer(_buf->vao, _buf->ebo);
}

FGLDEF void fgl_buffer_connect(fgl_buffer* _buf){
    glVertexArrayVertexBuffer(_buf->vao, 0, _buf->vbo, 0, sizeof(vertex));
}

FGLDEF void fgl_bind_arr(fgl_buffer *_buf){
    glBindVertexArray(_buf->vao);
}

FGLDEF void fgl_unbind_arr(){
    glBindVertexArray(0);
}

FGLDEF int fgl_data_static(fgl_buffer* _buf, size_t size, const void* data){
    if (_buf->vbo == 0){
        printf("error: vertex buffer is not valid\n");
        return 1;
    }
    if (_buf->ebo == 0){
        printf("error: vertex element buffer is not valid\n");
        return 1;
    }

    glNamedBufferData(_buf->vbo, size, data, GL_STATIC_DRAW);
    glNamedBufferData(_buf->ebo, size, data, GL_STATIC_DRAW);

    return 0;
}

FGLDEF int fgl_data_dynamic(fgl_buffer* _buf, size_t size, const void* data){
    if (_buf->vbo == 0){
        printf("error: vertex buffer is not valid\n");
        return 1;
    }
    if (_buf->ebo == 0){
        printf("error: vertex element buffer is not valid\n");
        return 1;
    }

    glNamedBufferData(_buf->vbo, size, data, GL_DYNAMIC_DRAW);
    glNamedBufferData(_buf->ebo, size, data, GL_DYNAMIC_DRAW);

    return 0;
}

FGLDEF int fgl_sub_data(fgl_buffer* _buf, size_t size, const void* data){
    if (_buf->vbo == 0){
        printf("error: vertex buffer is not valid\n");
        return 1;
    }
    if (_buf->ebo == 0){
        printf("error: vertex element buffer is not valid\n");
        return 1;
    }

    glNamedBufferSubData(_buf->vbo, 0, size, data);
    glNamedBufferSubData(_buf->ebo, 0, size, data);

    return 0;
}

FGLDEF void fgl_arr_attrib(fgl_buffer *_buf, int layout, int size, int offset){
    glEnableVertexArrayAttrib(_buf->vao, layout);
    glVertexArrayAttribFormat(
        _buf->vao,
        layout,
        size,
        GL_FLOAT, GL_FALSE,
        offset * sizeof(float)
    );
    glVertexArrayAttribBinding(_buf->vao, layout, 0);
}

FGLDEF int fgl_create_shader(uint32_t* shader, int target){
    *shader = glCreateShader(target);

    if (*shader == 0){
        printf("error: cannot create shader\n");
        return 1;
    }

    return 0;
}

FGLDEF int fgl_create_shader_prog(uint32_t* shader){
    *shader = glCreateProgram();

    if (*shader == 0){
        printf("error: cannot create program\n");
        return 1;
    }

    return 0;
}

FGLDEF char* fgl_load_shader_source(const char* s_path){
    return load_file(s_path);
}

FGLDEF int fgl_compile_shader_source(uint32_t shader, const char* path){
    glShaderSource(shader, 1, &path, NULL);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success){
        char log[512] = {};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        printf("shader log: %s", log);
        return 1;
    }

    return 0;
}

FGLDEF int fgl_attach_shader(uint32_t prog, uint32_t vs, uint32_t fs){
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    int success = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);

    if (!success){
        char log[512] = {};
        glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
        printf("program log: %s", log);
        return 1;
    }

    return 0;
}

FGLDEF void fgl_uniform_location(uint32_t shader, uint32_t* uniform, const char* loc){
    *uniform = glGetUniformLocation(shader, loc);
}

FGLDEF void fgl_uniform_mat(uint32_t *shader, uint32_t *loc, const float* value){
    glProgramUniformMatrix4fv(*shader, *loc, 1, GL_FALSE, value);
}

FGLDEF void fgl_use_prog(uint32_t *shader, uint32_t *uniform){
    glProgramUniform1i(*shader, *uniform, 0);
}

FGLDEF int fgl_create_texture(uint32_t* id, int target, int width, int height, void* pixels){
	glCreateTextures(target, 1, id);

	if (id == 0){
	    printf("error: failed to create texture\n");
		return 1;
	}

    glTextureStorage2D(*id, 1, GL_RGBA8, width, height);

    glTextureSubImage2D(*id,
        0, 0, 0, width, height,
        GL_RGBA, GL_UNSIGNED_BYTE, pixels
    );

	return 0;
}

FGLDEF void fgl_texture_filter(uint32_t id, int filter){
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, filter);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, filter);
}

FGLDEF void fgl_texture_repeat(uint32_t id){
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

FGLDEF void fgl_texture_bind(int unit, uint32_t id){
    glBindTextureUnit(unit, id);
}

FGLDEF void fgl_draw_elements(size_t count, void* offset){
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, offset);
}

FGLDEF void fgl_delete_buffer(uint32_t* buffer){
	if (buffer != 0) {
        glDeleteBuffers(1, buffer);
    }
}

FGLDEF void fgl_delete_arr(uint32_t* arr){
    if (arr != 0) {
        glDeleteVertexArrays(1, arr);
    }
}

FGLDEF void fgl_delete_prog(uint32_t prog){
    if (prog != 0) {
        glDeleteProgram(prog);
    }
}

FGLDEF void fgl_delete_shader(uint32_t shader){
    if (shader != 0) {
        glDeleteShader(shader);
    }
}

FGLDEF void fgl_delete_texture(uint32_t* texture){
    if (texture != 0){
        glDeleteTextures(1, texture);
    }
}

#endif // fgl_IMPL

#ifdef __cplusplus
}
#endif
