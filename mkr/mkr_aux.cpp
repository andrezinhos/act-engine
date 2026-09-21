#include "mkgl.hpp"
#include "mkr.hpp"

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

    printf("[INFO] GPU: %s\n", renderer);
    printf("[INFO] VENDOR: %s\n", vendor);

    return true;
}

void mkr::setWindowPosition(int width, int height){
    if (flags_active[2] == 0){
        mkr::wmain.moni = glfwGetPrimaryMonitor();
        mkr::wmain.mode = glfwGetVideoMode(mkr::wmain.moni);

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
    std::string vert_file = mkgl::loadShaderFile("eng/simple.vert");
    std::string frag_file = mkgl::loadShaderFile("eng/simple.frag");

    uint vs = mkgl::genShader(vert_file.c_str(), GL_VERTEX_SHADER);
    mkgl::compileShader(vs);

    uint fs = mkgl::genShader(frag_file.c_str(), GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);

    mkgl::genShaderProg(&shader.id, vs, fs);
    glGetUniformLocation(state.dshader.id, "uMvp");
    mkgl::deleteShaders(vs, fs);

    return shader;
}

Mesh mkr::DefaultQuad(){
    Mesh mesh = {};

    mesh.vertices = mkgl::SetNDC();
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    mkgl::genArrayBuffer(&mesh.vao);
    mkgl::genBuffer(&mesh.vbo);
    mkgl::genBuffer(&mesh.ebo);

    mkgl::bindArrBuff(&mesh.vao);

    mkgl::bindBuff(&mesh.vbo, GL_ARRAY_BUFFER);
    mkgl::bindDataStatic(GL_ARRAY_BUFFER, mesh.vertices.data(), mesh.vertices.size() * sizeof(vertex));

    mkgl::bindBuff(&mesh.ebo, GL_ELEMENT_ARRAY_BUFFER);
    mkgl::bindDataStatic(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.data(), mesh.indices.size() * sizeof(uint));

    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, MKR_VERTEX_STRIDE, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT,    4, MKR_VERTEX_STRIDE, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT,  2, MKR_VERTEX_STRIDE, 7);

    mkgl::unbind();

    return mesh;
}

void mkr::DefaultBatch(){
    mkgl::genArrayBuffer(&state.dbatch.vao);
    mkgl::genBuffer(&state.dbatch.vbo);
    mkgl::genBuffer(&state.dbatch.ebo);

    mkgl::bindArrBuff(&state.dbatch.vao);

    mkgl::bindBuff(&state.dbatch.vbo, GL_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ARRAY_BUFFER, nullptr, VMAX * sizeof(vertex));

    mkgl::bindBuff(&state.dbatch.ebo, GL_ELEMENT_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ELEMENT_ARRAY_BUFFER, nullptr, IMAX * sizeof(uint));

    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, MKR_VERTEX_STRIDE, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT,    4, MKR_VERTEX_STRIDE, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT,  2, MKR_VERTEX_STRIDE, 7);
    mkgl::unbind();

    state.dbatch.vertices.reserve(VMAX);
    state.dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::deleteProg(&state.dshader.id);
}

void mkr::UnloadDefaultQuad(){
    mkgl::deleteVertexArr(&state.dmesh.vao);
    mkgl::deleteBuffer(&state.dmesh.vbo);
    mkgl::deleteBuffer(&state.dmesh.ebo);
}

void mkr::UnloadDefaultBatch(){
    mkgl::deleteVertexArr(&state.dbatch.vao);
    mkgl::deleteBuffer(&state.dbatch.vbo);
    mkgl::deleteBuffer(&state.dbatch.ebo);

    state.dbatch.calls.clear();
    state.dbatch.indices.clear();
    state.dbatch.vertices.clear();
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, Vec2 uv){
    state.dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { uv.x, uv.x }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { uv.y, uv.x }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { uv.y, uv.y }});
    state.dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { uv.x, uv.y }});
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, float u0, float v0, float u1, float v1){
    state.dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { u0, v0 }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { u1, v0 }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { u1, v1 }});
    state.dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { u0, v1 }});
}

void mkr::sendIndices(uint base){
    state.dbatch.indices.push_back(base + 0);
    state.dbatch.indices.push_back(base + 1);
    state.dbatch.indices.push_back(base + 3);

    state.dbatch.indices.push_back(base + 1);
    state.dbatch.indices.push_back(base + 2);
    state.dbatch.indices.push_back(base + 3);
}

void mkr::drawElements(size_t count, void* offset){
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, offset);
}

void mkr::limitFlush(){
    if (state.dbatch.vertices.size() >= VMAX ||
        state.dbatch.indices.size() >= IMAX) flush();
}

void mkr::flush(){
    if (state.dbatch.vertices.empty()) return;

    mkgl::bindArrBuff(&state.dbatch.vao);

    // mkgl::bindBuff(&state.dbatch.vbo, GL_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ARRAY_BUFFER, nullptr, VMAX * sizeof(vertex));
    mkgl::bindSubData(
        GL_ARRAY_BUFFER, state.dbatch.vertices.data(),
        state.dbatch.vertices.size() * sizeof(vertex)
    );

    // mkgl::bindBuff(&state.dbatch.ebo, GL_ELEMENT_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ELEMENT_ARRAY_BUFFER, nullptr, IMAX * sizeof(uint));
    mkgl::bindSubData(
        GL_ELEMENT_ARRAY_BUFFER, state.dbatch.indices.data(),
        state.dbatch.indices.size() * sizeof(uint)
    );

    for (const auto& d : state.dbatch.calls){
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            reinterpret_cast<void*>(d.start * sizeof(uint))
        );
    }

    mkgl::unbind();

    state.dbatch.calls.clear();
    state.dbatch.indices.clear();
    state.dbatch.vertices.clear();
}
