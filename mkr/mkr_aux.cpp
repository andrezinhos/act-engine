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
    std::string vert_file = mkgl::loadShaderFile("eng/simple.vert");
    std::string frag_file = mkgl::loadShaderFile("eng/simple.frag");

    uint vs = mkgl::genShader(vert_file.c_str(), GL_VERTEX_SHADER);
    mkgl::compileShader(vs);

    uint fs = mkgl::genShader(frag_file.c_str(), GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);

    mkgl::genShaderProg(&shader.id, vs, fs);
    glGetUniformLocation(dstate->dshader.id, "uMvp");
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
    mkgl::genArrayBuffer(&dstate->dbatch.vao);
    mkgl::genBuffer(&dstate->dbatch.vbo);
    mkgl::genBuffer(&dstate->dbatch.ebo);

    mkgl::bindArrBuff(&dstate->dbatch.vao);

    mkgl::bindBuff(&dstate->dbatch.vbo, GL_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ARRAY_BUFFER, nullptr, VMAX * sizeof(vertex));

    mkgl::bindBuff(&dstate->dbatch.ebo, GL_ELEMENT_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ELEMENT_ARRAY_BUFFER, nullptr, IMAX * sizeof(uint));

    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, MKR_VERTEX_STRIDE, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT,    4, MKR_VERTEX_STRIDE, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT,  2, MKR_VERTEX_STRIDE, 7);
    mkgl::unbind();

    dstate->dbatch.vertices.reserve(VMAX);
    dstate->dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::deleteProg(&dstate->dshader.id);
}

void mkr::UnloadDefaultQuad(){
    mkgl::deleteVertexArr(&dstate->dmesh.vao);
    mkgl::deleteBuffer(&dstate->dmesh.vbo);
    mkgl::deleteBuffer(&dstate->dmesh.ebo);
}

void mkr::UnloadDefaultBatch(){
    mkgl::deleteVertexArr(&dstate->dbatch.vao);
    mkgl::deleteBuffer(&dstate->dbatch.vbo);
    mkgl::deleteBuffer(&dstate->dbatch.ebo);

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, Vec2 uv){
    dstate->dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { uv.x, uv.x }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { uv.y, uv.x }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { uv.y, uv.y }});
    dstate->dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { uv.x, uv.y }});
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, float u0, float v0, float u1, float v1){
    dstate->dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { u0, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b, color.a }, { u1, v0 }});
    dstate->dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { u1, v1 }});
    dstate->dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b, color.a }, { u0, v1 }});
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

    mkgl::bindArrBuff(&dstate->dbatch.vao);

    // mkgl::bindBuff(&dstate->dbatch.vbo, GL_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ARRAY_BUFFER, nullptr, VMAX * sizeof(vertex));
    mkgl::bindSubData(
        GL_ARRAY_BUFFER, dstate->dbatch.vertices.data(),
        dstate->dbatch.vertices.size() * sizeof(vertex)
    );

    // mkgl::bindBuff(&dstate->dbatch.ebo, GL_ELEMENT_ARRAY_BUFFER);
    mkgl::bindDataDynamic(GL_ELEMENT_ARRAY_BUFFER, nullptr, IMAX * sizeof(uint));
    mkgl::bindSubData(
        GL_ELEMENT_ARRAY_BUFFER, dstate->dbatch.indices.data(),
        dstate->dbatch.indices.size() * sizeof(uint)
    );

    for (const auto& d : dstate->dbatch.calls){
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            reinterpret_cast<void*>(d.start * sizeof(uint))
        );
    }

    mkgl::unbind();

    dstate->dbatch.calls.clear();
    dstate->dbatch.indices.clear();
    dstate->dbatch.vertices.clear();
}
