#include "mkr.hpp"

mkr::mkr(){
    mkgl::state.dshader = DefaultShader();
    mkgl::state.dmesh = DefaultQuad();
    DefaultBatch();
    printf("[INFO] DEFAULT STATE LOADED");
}

mkr::~mkr() {
    mkr::UnloadDefaultShader();
    mkr::UnloadDefaultQuad();
    mkr::UnloadDefaultBatch();
    printf("[INFO] DEFAULT STATE UNLOADED");
}

Shader mkr::DefaultShader() {
    Shader shader = {};
    std::string vert_file = mkgl::loadShaderFile("assets/shaders/simple.vert");
    std::string frag_file = mkgl::loadShaderFile("assets/shaders/simple.frag");

    const char *conv_vert = vert_file.c_str();
    const char *conv_frag = frag_file.c_str();

    uint vs = mkgl::genShader(conv_vert, GL_VERTEX_SHADER);
    mkgl::compileShader(vs);

    uint fs = mkgl::genShader(conv_frag, GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);

    shader.id = mkgl::genShaderProg(vs, fs);
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

    mkgl::sendAttribPtr(0, 3, 8, 0);
    mkgl::sendAttribPtr(1, 3, 8, 3);
    mkgl::sendAttribPtr(2, 2, 8, 6);

    mkgl::unbind();

    return mesh;
}

void mkr::DefaultBatch(){
    mkgl::state.dbatch.vao = mkgl::genBuff(types::arr);
    mkgl::state.dbatch.vbo = mkgl::genBuff(types::buff);
    mkgl::state.dbatch.ebo = mkgl::genBuff(types::element);

    mkgl::bindBuff(mkgl::state.dbatch.vao, types::arr);

    mkgl::bindBuff(mkgl::state.dbatch.vbo, types::buff);
    mkgl::bindDataDynamic(types::buff, nullptr, VMAX * sizeof(vertex));

    mkgl::bindBuff(mkgl::state.dbatch.ebo, types::element);
    mkgl::bindDataDynamic(types::element, nullptr, IMAX * sizeof(uint));

    mkgl::sendAttribPtr(0, 3, 8, 0);
    mkgl::sendAttribPtr(1, 3, 8, 3);
    mkgl::sendAttribPtr(2, 2, 8, 6);

    mkgl::unbind();

    mkgl::state.dbatch.vertices.reserve(VMAX);
    mkgl::state.dbatch.indices.reserve(IMAX);
}

void mkr::UnloadDefaultShader(){
    mkgl::Delete(mkgl::state.dshader.id, types::prog);
}

void mkr::UnloadDefaultQuad(){
    mkgl::Delete(mkgl::state.dmesh.vao, types::arr);
    mkgl::Delete(mkgl::state.dmesh.vbo, types::buff);
    mkgl::Delete(mkgl::state.dmesh.ebo, types::element);
}

void mkr::UnloadDefaultBatch(){
    mkgl::Delete(mkgl::state.dbatch.vao, types::arr);
    mkgl::Delete(mkgl::state.dbatch.vbo, types::buff);
    mkgl::Delete(mkgl::state.dbatch.ebo, types::element);

    mkgl::state.dbatch.calls.clear();
    mkgl::state.dbatch.indices.clear();
    mkgl::state.dbatch.vertices.clear();
}
