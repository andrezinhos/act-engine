#include "mktxt.hpp"
#include "mktex.hpp"
#include "mkr.hpp"
#include "stb_image.h"

Window wmain;
DState* dstate = nullptr;
int flags_active[3];

void frameCallback(GLFWwindow* window, int w, int h){
	if (w == 0 || h == 0) return;

    glViewport(0, 0, w, h);
    wmain.win_width = w;
    wmain.win_height = h;
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

	if (flags_active[2] == 1) {
		wmain.moni = glfwGetPrimaryMonitor();
		wmain.mode = glfwGetVideoMode(wmain.moni);

		glfwWindowHint(GLFW_RED_BITS, wmain.mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, wmain.mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, wmain.mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, wmain.mode->refreshRate);
	}

    if (flags_active[0] == 1) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    wmain.main = glfwCreateWindow(width, height, title, wmain.moni, nullptr);
    if (flags_active[1] == 1) glfwMaximizeWindow(wmain.main);
    if (!wmain.main){
        printf("Error to Create Window");
        glfwTerminate();
        return false;
    }

    setWindowPosition(width, height);
    glfwSetFramebufferSizeCallback(wmain.main, frameCallback);
    createWindowContext();

    mkgl::enableBlend(true);

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
	return wmain.win_width;
}

int mkr::GetWindowHeight(){
	return wmain.win_height;
}

void mkr::Initialize(){
    dstate = new DState();

    dstate->dshader = DefaultShader();
    dstate->dmesh = DefaultQuad();
    dstate->dtex = mktex::DefaultTexture();
    dstate->dfont = mktxt::DefaultFont();
    DefaultBatch();
    printf("[INFO] DEFAULT STATE LOADED\n");
}

void mkr::Shutdown(){
    mkr::UnloadDefaultBatch();
    mktxt::UnloadDefaultFont();
    mktex::UnloadDefaultTexture();
    mkr::UnloadDefaultQuad();
    mkr::UnloadDefaultShader();

    delete dstate;
    dstate = nullptr;
    printf("[INFO] DEFAULT STATE UNLOADED\n");
    glfwDestroyWindow(wmain.main);
    glfwTerminate();
}

Shader mkr::LoadShader(cstr vsPath, cstr fsPath) {
    Shader shader = {};
    char* vert_file = mkgl::loadShaderFile(vsPath);
    char* frag_file = mkgl::loadShaderFile(fsPath);

    uint vs = mkgl::genShader(vert_file, GL_VERTEX_SHADER);
    mkgl::compileShader(vs);
    freeptr(vert_file);

    uint fs = mkgl::genShader(frag_file, GL_FRAGMENT_SHADER);
    mkgl::compileShader(fs);
    freeptr(frag_file);

    mkgl::genShaderProg(&shader.id, vs, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return shader;
}

void mkr::ScreenClear(Color color){
    mkgl::clearScreen(color);
}

void mkr::RenderBegin(){
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(dstate->dshader.id);
    glUniform1i(dstate->dshader.utex, 0);
}

void mkr::RenderEnd(){
    flush();
    glfwPollEvents();
    glfwSwapBuffers(wmain.main);
}

void mkr::CameraBegin(Camera2D& camera){
    Matrix proj = mkmath::GetProjectionMatrix(wmain.win_width, wmain.win_height);
    Matrix view = mkmath::GetViewMatrix(camera);
    Matrix model = MatrixIdentity();

    Matrix mvp = mkmath::MultiplyMatrix(mkmath::MultiplyMatrix(proj, view), model);
    mkgl::setUniformMat(dstate->dshader.umodel, mvp);
}

void mkr::CameraEnd(){
    flush();
    Matrix view = MatrixIdentity();
    mkgl::setUniformMat(dstate->dshader.uview, view);
}
