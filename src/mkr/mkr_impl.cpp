#include "mkr.hpp"

Window mkr::wmain;

int mkr::flags_active[4];

void frameCallback(GLFWwindow* window, int w, int h){
	if (w == 0 || h == 0) return;

    glViewport(0, 0, w, h);
    mkr::wmain.win_width = w;
    mkr::wmain.win_height = h;
}

bool mkr::startWindow(int width, int height, const char* title){

	if (flags_active[3] == 1) {
		mkr::wmain.moni = glfwGetPrimaryMonitor();
		mkr::wmain.mode = glfwGetVideoMode(mkr::wmain.moni);

		glfwWindowHint(GLFW_RED_BITS, wmain.mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, wmain.mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, wmain.mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, wmain.mode->refreshRate);
	}

    if (flags_active[1] == 1) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    wmain.main = glfwCreateWindow(width, height, title, wmain.moni, nullptr);
    if (flags_active[2] == 1) glfwMaximizeWindow(wmain.main);

    if (!wmain.main){
        printf("Error to Create Window");
        glfwTerminate();
        return false;
    }
    glfwSetFramebufferSizeCallback(wmain.main, frameCallback);

    glfwMakeContextCurrent(wmain.main);
    if (flags_active[0] == 1) glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load OpenGL Context");
        glfwDestroyWindow(wmain.main);
        glfwTerminate();
        return false;
    }

	// this is for in case of wrong viewport
	// on start of the window, specially in the maximized flag
    int fb_w, fb_h;
    glfwGetFramebufferSize(wmain.main, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    wmain.win_width = fb_w;
    wmain.win_height = fb_h;

    return true;
}

void mkr::Initialize(){
    mkgl::state.dshader = DefaultShader();
    mkgl::state.dmesh = DefaultQuad();
    DefaultBatch();
    printf("[INFO] DEFAULT STATE LOADED\n");
}

void mkr::Shutdown(){
    mkr::UnloadDefaultShader();
    mkr::UnloadDefaultQuad();
    mkr::UnloadDefaultBatch();
    printf("[INFO] DEFAULT STATE UNLOADED\n");
    glfwDestroyWindow(mkr::wmain.main);
    glfwTerminate();
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

Texture mkr::LoadTextureSrc(const char* path){
    Image image = mkgl::loadImage(path);
    Texture tex;
    tex.id = mkgl::genTex(GL_TEXTURE_2D);

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mkgl::setTexImage2D(GL_RGBA, image.width, image.height, image.data);
    mkgl::unloadImage(image);

    return tex;
}

void mkr::UnloadTexture(const Texture& tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("Texture Unloaded\n");
}

void mkr::ScreenClear(Color color){
    mkgl::clearScreen(color);
}

void mkr::RenderTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    if (mkgl::state.dbatch.vertices.size() + 4  > VMAX || mkgl::state.dbatch.indices.size() + 6  > IMAX) mkgl::flush();
    uint32_t base = mkgl::state.dbatch.vertices.size();
    uint32_t indexStart = mkgl::state.dbatch.indices.size();

    mkgl::state.dbatch.vertices.push_back({{ position.x,          position.y, 0.0f },{ color.r, color.g, color.b }, { 0.0f, 0.0f }});
    mkgl::state.dbatch.vertices.push_back({{ position.x + size.x, position.y, 0.0f},{ color.r, color.g, color.b }, { 1.0f, 0.0f }});
    mkgl::state.dbatch.vertices.push_back({{ position.x + size.x, position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 1.0f, 1.0f }});
    mkgl::state.dbatch.vertices.push_back({{ position.x,          position.y + size.y, 0.0f },{ color.r, color.g, color.b }, { 0.0f, 1.0f }});

    mkgl::state.dbatch.indices.push_back(base + 0);
    mkgl::state.dbatch.indices.push_back(base + 1);
    mkgl::state.dbatch.indices.push_back(base + 3);

    mkgl::state.dbatch.indices.push_back(base + 1);
    mkgl::state.dbatch.indices.push_back(base + 2);
    mkgl::state.dbatch.indices.push_back(base + 3);

    if (mkgl::state.dbatch.calls.empty() || mkgl::state.dbatch.calls.back().texref != tex){
        mkgl::state.dbatch.calls.push_back({indexStart, 6, tex});
    }
    else mkgl::state.dbatch.calls.back().count += 6;
}

void mkr::RenderBegin(){
    glUseProgram(mkgl::state.dshader.id);
    glUniform1i(mkgl::state.dshader.utex, 0);
}

void mkr::RenderEnd(){
    mkgl::flush();
    glfwSwapBuffers(wmain.main);
}

void mkr::CameraBegin(Camera2D& camera){
    Matrix proj = gmath::GetProjectionMatrix(wmain.win_width, wmain.win_height);
    Matrix view = gmath::GetViewMatrix(camera);
    Matrix model = Matrix::Identity();

    Matrix mvp = gmath::MultiplyMatrix(gmath::MultiplyMatrix(proj, view), model);
    mkgl::setUniformMat(mkgl::state.dshader.umodel, mvp);
}

void mkr::CameraEnd(){
    mkgl::flush();
    Matrix view = Matrix::Identity();
    mkgl::setUniformMat(mkgl::state.dshader.uview, view);
}
