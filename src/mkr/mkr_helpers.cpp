#include "mkgl.hpp"
#include "mkr.hpp"

bool mkr::createWindowContext(){
    glfwMakeContextCurrent(wmain.main);
    if (flags_active[0] == 1) glfwSwapInterval(1);

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
    if (flags_active[3] == 0){
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
    std::string vert_file = mkgl::loadShaderFile("assets/shaders/simple.vert");
    std::string frag_file = mkgl::loadShaderFile("assets/shaders/simple.frag");

    uint vs = mkgl::genShader(vert_file.c_str(), GL_VERTEX_SHADER);
    mkgl::compileShader(vs);

    uint fs = mkgl::genShader(frag_file.c_str(), GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);

    shader.id = mkgl::genShaderProg(vs, fs);

    glGetUniformLocation(state.dshader.id, "uMvp");
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    return shader;
}

Mesh mkr::DefaultQuad(){
    Mesh mesh = {};

    mesh.vertices = mkgl::SetNDC();
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };

    mesh.vao = mkgl::genBuff(types::arr);
    mesh.vbo = mkgl::genBuff(types::buff);
    mesh.ebo = mkgl::genBuff(types::element);

    mkgl::bindBuff(mesh.vao, types::arr);

    mkgl::bindBuff(mesh.vbo, types::buff);
    mkgl::bindDataStatic(types::buff, mesh.vertices.data(), mesh.vertices.size() * sizeof(vertex));

    mkgl::bindBuff(mesh.ebo, types::element);
    mkgl::bindDataStatic(types::element, mesh.indices.data(), mesh.indices.size() * sizeof(uint));

    mkgl::sendAttribPtr(MKR_POSITION_LAYOUT, 3, 8, 0);
    mkgl::sendAttribPtr(MKR_COLOR_LAYOUT, 3, 8, 3);
    mkgl::sendAttribPtr(MKR_TEXTURE_LAYOUT, 2, 8, 6);

    mkgl::unbind();

    return mesh;
}

Texture mkr::DefaultTexture(){
    Texture tex;
    tex.id = mkgl::genTex(GL_TEXTURE_2D);

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint color = 0xFFFFFFFF;
    mkgl::setTexImage2D(GL_RGBA, GL_RGBA, 1, 1, &color);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void mkr::DefaultBatch(){
    state.dbatch.vao = mkgl::genBuff(types::arr);
    state.dbatch.vbo = mkgl::genBuff(types::buff);
    state.dbatch.ebo = mkgl::genBuff(types::element);

    mkgl::bindBuff(state.dbatch.vao, types::arr);

    mkgl::bindBuff(state.dbatch.vbo, types::buff);
    mkgl::bindDataDynamic(types::buff, nullptr, VMAX * sizeof(vertex));

    mkgl::bindBuff(state.dbatch.ebo, types::element);
    mkgl::bindDataDynamic(types::element, nullptr, IMAX * sizeof(uint));

    mkgl::sendAttribPtr(0, 3, 8, 0);
    mkgl::sendAttribPtr(1, 3, 8, 3);
    mkgl::sendAttribPtr(2, 2, 8, 6);

    mkgl::unbind();

    state.dbatch.vertices.reserve(VMAX);
    state.dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::Delete(state.dshader.id, types::prog);
}

void mkr::UnloadDefaultQuad(){
    mkgl::Delete(state.dmesh.vao, types::arr);
    mkgl::Delete(state.dmesh.vbo, types::buff);
    mkgl::Delete(state.dmesh.ebo, types::element);
}

void mkr::UnloadDefaultTexture(){
    if (state.dtex.id != 0) glDeleteTextures(1, &state.dtex.id);
}

void mkr::UnloadDefaultFont(){
    if(state.dfont.fontTex.id != 0) glDeleteTextures(1, &state.dfont.fontTex.id);
}

void mkr::UnloadDefaultBatch(){
    mkgl::Delete(state.dbatch.vao, types::arr);
    mkgl::Delete(state.dbatch.vbo, types::buff);
    mkgl::Delete(state.dbatch.ebo, types::element);

    state.dbatch.calls.clear();
    state.dbatch.indices.clear();
    state.dbatch.vertices.clear();
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, Vec2 uv){
    state.dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b }, { uv.x, uv.x }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { uv.y, uv.x }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { uv.y, uv.y }});
    state.dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { uv.x, uv.y }});
}

void mkr::sendVertex(Vec2 position, Vec2 size, Color color, float u0, float v0, float u1, float v1){
    state.dbatch.vertices.push_back({{ position.x,          position.y,          0.0f },{ color.r, color.g, color.b }, { u0, v0 }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y,          0.0f },{ color.r, color.g, color.b }, { u1, v0 }});
    state.dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u1, v1 }});
    state.dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { u0, v1 }});
}

void mkr::sendIndices(unsigned int base){
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
    size_t max_v = state.dbatch.vertices.size() + 4;
    size_t max_i = state.dbatch.indices.size() + 6;
    if (max_v < VMAX || max_i < IMAX) return;
    else flush();
}

void mkr::flush(){
    if (state.dbatch.vertices.empty()) return;

    mkgl::bindBuff(state.dbatch.vao, types::arr);

    mkgl::bindBuff(state.dbatch.vbo, types::buff);
    mkgl::bindSubData(
        types::buff, state.dbatch.vertices.data(),
        state.dbatch.vertices.size() * sizeof(vertex)
    );

    mkgl::bindBuff(state.dbatch.ebo, types::element);
    mkgl::bindSubData(
        types::element, state.dbatch.indices.data(),
        state.dbatch.indices.size() * sizeof(uint32_t)
    );

    for (const auto& d : state.dbatch.calls){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, d.texref->id);

        drawElements(
            d.count,
            (void*)(d.start * sizeof(uint))
        );
    }

    state.dbatch.calls.clear();
    state.dbatch.indices.clear();
    state.dbatch.vertices.clear();
}

