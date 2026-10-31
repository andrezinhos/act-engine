#include "fgl.h"
#include "mkr.hpp"
#include <cstdio>
#include <cstdlib>

bool mkr::createWindowContext(){
    glfwMakeContextCurrent(wmain.main);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load OpenGL functions");
        glfwDestroyWindow(wmain.main);
        glfwTerminate();
        return false;
    }

    fgl_hardware_info();

    return true;
}

void mkr::setWindowPosition(int width, int height){
    if (flags_active[2] == 0){
        wmain.moni = glfwGetPrimaryMonitor();
        wmain.mode = glfwGetVideoMode(wmain.moni);

        int monitorX = wmain.mode->width;
        int monitorY = wmain.mode->height;
        printf("[INFO] MONITOR SIZE: %d | %d \n", monitorX, monitorY);

        int centerX = (monitorX/2) - (width/2);
        int centerY = (monitorY/2) - (height/2);
        glfwSetWindowPos(wmain.main, centerX, centerY);
    }
}

Shader mkr::DefaultShader() {
    Shader shader = {};

    char* vert_file = fgl_load_shader_source("eng/simple.vert");
    char* frag_file = fgl_load_shader_source("eng/simple.frag");

    uint32_t vs = 0;
    uint32_t fs = 0;

    fgl_create_shader(&vs, GL_VERTEX_SHADER);
    fgl_create_shader(&fs, GL_FRAGMENT_SHADER);

    fgl_compile_shader_source(vs, vert_file);
    free(vert_file);
    vert_file = NULL;

    fgl_compile_shader_source(fs, frag_file);
    free(frag_file);
    frag_file = NULL;

    fgl_create_shader_prog(&shader.id);
    fgl_attach_shader(shader.id, vs, fs);

    fgl_uniform_location(shader.id, &shader.umodel, "uMvp");
    fgl_uniform_location(shader.id, &shader.utex, "uTexture");

    fgl_uniform_prog(&shader.id, &shader.umodel);
    fgl_uniform_prog(&shader.id, &shader.utex);

    fgl_delete_shader(vs);
    fgl_delete_shader(fs);

    return shader;
}

Mesh mkr::DefaultQuad(){
    Mesh mesh = {};

    mesh.vertices.reserve(4);

    mkgl::set_initial_vertex(mesh.vertices.data());
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    mkgl::gen_array(&mesh.buffer.vao);
    mkgl::gen_buffer(&mesh.buffer.vbo);
    mkgl::gen_buffer(&mesh.buffer.ebo);

    mkgl::bind_arr_buf(&mesh.buffer.vao);

    mkgl::bind_data_static(
        &mesh.buffer.vbo,
        mesh.vertices.data(),
        vertex_size(mesh.vertices.size()), GL_ARRAY_BUFFER
    );

    mkgl::bind_data_static(
        &mesh.buffer.ebo,
        mesh.indices.data(),
        element_size(mesh.indices.size()), GL_ELEMENT_ARRAY_BUFFER
    );

    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, MKR_VERTEX_STRIDE, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT,    3, MKR_VERTEX_STRIDE, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT,  2, MKR_VERTEX_STRIDE, 6);

    mkgl::arr_unbind();

    return mesh;
}

void mkr::DefaultBatch(){
    mkgl::create_array(&dstate->dbatch.vao);
    mkgl::create_buffer(&dstate->dbatch.vbo);
    mkgl::create_buffer(&dstate->dbatch.ebo);

    mkgl::element_connect(&dstate->dbatch.vao, &dstate->dbatch.ebo);

    mkgl::buf_data_dynamic(&dstate->dbatch.vbo, nullptr, VMAX * sizeof(vertex));
    mkgl::buf_data_dynamic(&dstate->dbatch.ebo, nullptr, IMAX * sizeof(uint32_t));

    mkgl::set_attrib_vertex_buffer(&dstate->dbatch.vao, &dstate->dbatch.vbo, MKR_VERTEX_STRIDE);

    mkgl::set_attrib_format(&dstate->dbatch.vao, MKR_POSITION_LAYOUT, 3, 0);
    mkgl::bind_vertex_attrib(&dstate->dbatch.vao, 0, MKR_POSITION_LAYOUT);
    mkgl::enable_array_attrib(&dstate->dbatch.vao, MKR_POSITION_LAYOUT);

    mkgl::set_attrib_format(&dstate->dbatch.vao, MKR_COLOR_LAYOUT, 3, 3);
    mkgl::bind_vertex_attrib(&dstate->dbatch.vao, 0, MKR_COLOR_LAYOUT);
    mkgl::enable_array_attrib(&dstate->dbatch.vao, MKR_COLOR_LAYOUT);

    mkgl::set_attrib_format(&dstate->dbatch.vao, MKR_TEXTURE_LAYOUT, 2, 6);
    mkgl::bind_vertex_attrib(&dstate->dbatch.vao, 0, MKR_TEXTURE_LAYOUT);
    mkgl::enable_array_attrib(&dstate->dbatch.vao, MKR_TEXTURE_LAYOUT);

    dstate->dbatch.vertices.reserve(VMAX);
    dstate->dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::del_prog(&dstate->dshader.id);
}

void mkr::UnloadDefaultQuad(){
    mkgl::del_arr(&dstate->dmesh.buffer.vao);
    mkgl::del_buffer(&dstate->dmesh.buffer.vbo);
    mkgl::del_buffer(&dstate->dmesh.buffer.ebo);
}

void mkr::UnloadDefaultBatch(){
    mkgl::del_arr(&dstate->dbatch.vao);
    mkgl::del_buffer(&dstate->dbatch.vbo);
    mkgl::del_buffer(&dstate->dbatch.ebo);

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}

void mkr::send_vertex(Vec2 position, Vec2 size, color color, float u0, float v0, float u1, float v1){
    dstate->dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b }, { u0, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { u1, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u1, v1 }});
    dstate->dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u0, v1 }});
}

void mkr::sendIndices(uint32_t base){
    dstate->dbatch.indices.push_back(base + 0);
    dstate->dbatch.indices.push_back(base + 1);
    dstate->dbatch.indices.push_back(base + 3);

    dstate->dbatch.indices.push_back(base + 1);
    dstate->dbatch.indices.push_back(base + 2);
    dstate->dbatch.indices.push_back(base + 3);
}

void mkr::drawElements(size_t count, void* offset){
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, offset);
}

void mkr::limitFlush(){
    if (dstate->dbatch.vertices.size() >= VMAX ||
        dstate->dbatch.indices.size() >= IMAX) flush();
}

void mkr::flush(){
    if (dstate->dbatch.vertices.empty()) return;

    mkgl::buf_sub_data(
        &dstate->dbatch.vbo, dstate->dbatch.vertices.data(),
        dstate->dbatch.vertices.size() * sizeof(vertex)
    );

    mkgl::buf_sub_data(
        &dstate->dbatch.ebo, dstate->dbatch.indices.data(),
        dstate->dbatch.indices.size() * sizeof(uint32_t)
    );

    mkgl::bind_arr_buf(&dstate->dbatch.vao);
    for (const auto& d : dstate->dbatch.calls){
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            reinterpret_cast<void*>(d.start * sizeof(uint32_t))
        );
    }
    mkgl::arr_unbind();

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}
