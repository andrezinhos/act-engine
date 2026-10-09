#include "mkgl.hpp"
#include "mkr.hpp"
#include <cstdlib>

bool mkr::createWindowContext(){
    glfwMakeContextCurrent(wmain.main);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load OpenGL Context");
        glfwDestroyWindow(wmain.main);
        glfwTerminate();
        return false;
    }

    const byte* vendor = glGetString(GL_VENDOR);
    const byte* renderer = glGetString(GL_RENDERER);
    const byte* glver = glGetString(GL_VERSION);

    printf("[INFO] GPU: %s\n", renderer);
    printf("[INFO] VENDOR: %s\n", vendor);
    printf("[INFO] OPENGL VERSION: %s\n", glver);

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
    char* vert_file = mkgl::loadShaderFile("eng/simple.vert");
    char* frag_file = mkgl::loadShaderFile("eng/simple.frag");

    uint vs, fs;

    mkgl::create_shader(&vs, vert_file, GL_VERTEX_SHADER);
    free(vert_file);
    vert_file = NULL;

    mkgl::create_shader(&fs, frag_file, GL_FRAGMENT_SHADER);
    free(frag_file);
    frag_file = NULL;

    mkgl::gen_shader_prog(&shader.id, vs, fs);
    glGetUniformLocation(dstate->dshader.id, "uMvp");
    mkgl::delete_shaders(vs, fs);

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

    mkgl::gen_array(&mesh.vao);
    mkgl::gen_buffer(&mesh.vbo);
    mkgl::gen_buffer(&mesh.ebo);

    mkgl::bind_arr_buf(&mesh.vao);

    mkgl::bind_data_static(
        &mesh.vbo,
        mesh.vertices.data(),
        mesh.vertices.size() * sizeof(vertex), GL_ARRAY_BUFFER
    );

    mkgl::bind_data_static(
        &mesh.ebo,
        mesh.indices.data(),
        mesh.indices.size() * sizeof(uint), GL_ELEMENT_ARRAY_BUFFER
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
    mkgl::buf_data_dynamic(&dstate->dbatch.ebo, nullptr, IMAX * sizeof(uint));

    mkgl::bind_arr_buf(&dstate->dbatch.vao);
    mkgl::bind_buf(&dstate->dbatch.vbo);
    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, MKR_VERTEX_STRIDE, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT,    3, MKR_VERTEX_STRIDE, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT,  2, MKR_VERTEX_STRIDE, 6);
    mkgl::buf_unbind();
    mkgl::arr_unbind();

    dstate->dbatch.vertices.reserve(VMAX);
    dstate->dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::del_prog(&dstate->dshader.id);
}

void mkr::UnloadDefaultQuad(){
    mkgl::del_arr(&dstate->dmesh.vao);
    mkgl::del_buffer(&dstate->dmesh.vbo);
    mkgl::del_buffer(&dstate->dmesh.ebo);
}

void mkr::UnloadDefaultBatch(){
    mkgl::del_arr(&dstate->dbatch.vao);
    mkgl::del_buffer(&dstate->dbatch.vbo);
    mkgl::del_buffer(&dstate->dbatch.ebo);

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, Vec2 uv){
    dstate->dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b }, { uv.x, uv.x }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { uv.y, uv.x }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { uv.y, uv.y }});
    dstate->dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { uv.x, uv.y }});
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, float u0, float v0, float u1, float v1){
    dstate->dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b }, { u0, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { u1, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u1, v1 }});
    dstate->dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u0, v1 }});
}

void mkr::sendIndices(uint base){
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
        dstate->dbatch.indices.size() * sizeof(uint)
    );

    mkgl::bind_arr_buf(&dstate->dbatch.vao);
    for (const auto& d : dstate->dbatch.calls){
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            reinterpret_cast<void*>(d.start * sizeof(uint))
        );
    }
    mkgl::arr_unbind();

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}
