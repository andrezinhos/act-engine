#include "mkgl.hpp"
#include "mkr.hpp"
#include "stb_image.h"

Window mkr::wmain;
int mkr::flags_active[4];

void frameCallback(GLFWwindow* window, int w, int h){
	if (w == 0 || h == 0) return;

    glViewport(0, 0, w, h);
    mkr::wmain.win_width = w;
    mkr::wmain.win_height = h;
}

void mkr::setWindowIcon(const char* path){
    wmain.icon.pixels = stbi_load(path, &wmain.icon.width, &wmain.icon.height, 0, 4);
    glfwSetWindowIcon(wmain.main, 1, &wmain.icon);
    stbi_image_free(wmain.icon.pixels);
}

void mkr::setCursorMode(Cursor cur){
    if (cur == NORMAL) glfwSetInputMode(wmain.main, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (cur == HIDDEN) glfwSetInputMode(wmain.main, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (cur == DISABLED) glfwSetInputMode(wmain.main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    setWindowPosition(width, height);

    glfwSetFramebufferSizeCallback(wmain.main, frameCallback);

    createWindowContext();

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// this is for in case of wrong viewport
	// on start of the window, specially in the maximized flag
    int fb_w, fb_h;
    glfwGetFramebufferSize(wmain.main, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);
    wmain.win_width = fb_w;
    wmain.win_height = fb_h;

    return true;
}

int mkr::GetWindowWidth(){
	return mkr::wmain.win_width;
}

int mkr::GetWindowHeight(){
	return mkr::wmain.win_height;
}

void mkr::Initialize(){
    mkgl::state.dshader = DefaultShader();
    mkgl::state.dmesh = DefaultQuad();
    mkgl::state.dtex = DefaultTexture();
    mkgl::state.dfont = DefaultFont();
    DefaultBatch();
    printf("[INFO] DEFAULT STATE LOADED\n");
}

void mkr::Shutdown(){
    mkr::UnloadDefaultBatch();
    mkr::UnloadDefaultTexture();
    mkr::UnloadDefaultQuad();
    mkr::UnloadDefaultShader();
    printf("[INFO] DEFAULT STATE UNLOADED\n");
    glfwDestroyWindow(mkr::wmain.main);
    glfwTerminate();
}

Shader mkr::LoadShader(const char* vsPath, const char* fsPath) {
    Shader shader = {};
    std::string vert_file = mkgl::loadShaderFile(vsPath);
    std::string frag_file = mkgl::loadShaderFile(fsPath);

    uint vs = mkgl::genShader(vert_file.c_str(), GL_VERTEX_SHADER);
    mkgl::compileShader(vs);

    uint fs = mkgl::genShader(frag_file.c_str(), GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);

    shader.id = mkgl::genShaderProg(vs, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return shader;
}

Texture mkr::LoadTextureSrc(const char* path){
    Image image = mkgl::loadImage(path);
    Texture tex;
    tex.id = mkgl::genTex(GL_TEXTURE_2D);
    tex.width = image.width;
    tex.height = image.height;

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mkgl::setTexImage2D(GL_RGBA, GL_RGBA, image.width, image.height, image.data);
    mkgl::unloadImage(image);

    return tex;
}

void mkr::UnloadTexture(const Texture& tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("[INFO] TEXTURE UNLOADED\n");
}

void mkr::ScreenClear(Color color){
    mkgl::clearScreen(color);
}

void mkr::RenderRectangle(Vec2 position, Vec2 size, Color color){
    mkgl::limitFlush();
    uint32_t base = mkgl::state.dbatch.vertices.size();
    uint32_t indexStart = mkgl::state.dbatch.indices.size();

    mkgl::sendVertex(position, size, color, {0.0f, 1.0f});
    mkgl::sendIndices(base);
    
    if (mkgl::state.dbatch.calls.empty() || mkgl::state.dbatch.calls.back().texref != &mkgl::state.dtex){
        mkgl::state.dbatch.calls.push_back({indexStart, 6, &mkgl::state.dtex});
    }
    else mkgl::state.dbatch.calls.back().count += 6;
}

/*

UV Calc
UV 0 = rect_x / tex_width
UV 1 = (rect_x + rect_width) / tex_width

*/

void mkr::RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, Color color){
    mkgl::limitFlush();
    uint base = mkgl::state.dbatch.vertices.size();
    uint indexStart = mkgl::state.dbatch.indices.size();

    float u0 = rectangle.x / (float)tex->width;
    float v0 = rectangle.y / (float)tex->height;

    float u1 = (rectangle.x + (float)rectangle.width) / (float)tex->width;
    float v1 = (rectangle.y + (float)rectangle.height) / (float)tex->height;

    mkgl::sendVertex(position, size, color, u0, v0, u1, v1);
    mkgl::sendIndices(base);

    if (mkgl::state.dbatch.calls.empty() || mkgl::state.dbatch.calls.back().texref != tex){
        mkgl::state.dbatch.calls.push_back({indexStart, 6, tex});
    }
    else mkgl::state.dbatch.calls.back().count += 6;
}

void mkr::RenderTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    mkgl::limitFlush();
    uint base = mkgl::state.dbatch.vertices.size();
    uint indexStart = mkgl::state.dbatch.indices.size();

    mkgl::sendVertex(position, size, color, {0.0f, 1.0f});
    mkgl::sendIndices(base);

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
