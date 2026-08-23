#include "core.hpp"
#include "glfw/glfw3.h"
#include "gmath.hpp"
#include "mkr.hpp"
#include "utils.hpp"
#include <cstdio>

constexpr const char* VERSION = "0.6.0";
CoreState core::state = {};

/*
void frameCallback(GLFWwindow* window, int w, int h){
    glViewport(0, 0, w, h);
}
*/

void UnloadDefaultShader(){
    if (core::state.dshader.id != 0) glDeleteProgram(core::state.dshader.id);
    printf("Default Shader Unloaded\n");
}

void UnloadDefaultQuad(){
    if (core::state.dmesh.vao != 0 && core::state.dmesh.vbo != 0 && core::state.dmesh.ebo != 0){
        glDeleteVertexArrays(1, &core::state.dmesh.vao);
        glDeleteBuffers(1, &core::state.dmesh.vbo);
        glDeleteBuffers(1, &core::state.dmesh.ebo);
    }
}

void UnloadDefaultTexture(){
    if (core::state.dtex.id != 0) glDeleteTextures(1, &core::state.dtex.id);
}

void UnloadDefaultBatch(){
    if (mkr::batch.vao != 0 && mkr::batch.vbo != 0 && mkr::batch.ebo != 0){
        glDeleteVertexArrays(1, &mkr::batch.vao);
        glDeleteBuffers(1, &mkr::batch.vbo);
        glDeleteBuffers(1, &mkr::batch.ebo);
    }
}

void core::LoadDefault(){
    state.dshader = mkr::DefaultShader();
    state.dmesh = mkr::DefaultQuad();
    state.dtex = mkr::DefaultTexture();
    mkr::DefaultBatch();
}

void core::UnloadDefault(){
    UnloadDefaultBatch();
    UnloadDefaultTexture();
    UnloadDefaultQuad();
    UnloadDefaultShader();
}

void core::WindowFlag(Flags flag){
    switch(flag){
        case VSYNC: state.flags_active[0] = 1; break;
        case RESIZABLE: state.flags_active[1] = 1; break;
        case MAXIMIZED: state.flags_active[2] = 1; break;
    }
}

void core::init(){
    bool has_init = glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (has_init) printf("ACT ENGINE v%s\n", VERSION);
}

void core::TargetFPS(double fps){
    state.targetfps = fps;
    state.duration = 1.0f / fps;
}

float core::GetDelta(){
    return state.delta;
}

bool startWindow(int width, int height, const char* title){
    if (core::state.flags_active[1] == 1) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    core::state.win = glfwCreateWindow(width, height, title, NULL, NULL);
    if (core::state.flags_active[2] == 1) glfwMaximizeWindow(core::state.win);

    if (!core::state.win){
        printf("Error to Create Window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(core::state.win);
    if (core::state.flags_active[0] == 1) glfwSwapInterval(1);

    /* for some reason, this makes the camera useless, so for now is off */
    // glfwSetFramebufferSizeCallback(win, frameCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load OpenGL Context");
        glfwDestroyWindow(core::state.win);
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, width, height);
    return true;
}

void core::MainWindow(int width, int height, const char *title){
    core::init();
    core::state.win_width = width;
    core::state.win_height = height;
    bool win_started = startWindow(width, height, title);

    if (win_started){
        LoadDefault();
        state.lastTime = glfwGetTime();
        printf("[INFO] ENGINE INITIALIZED\n");
    }
}

void core::DrawBegin(){
    glUseProgram(state.dshader.id);
    glUniform1i(state.dshader.utex, 0);
}

void core::DrawEnd(){
    mkr::Flush();
    glfwSwapBuffers(core::state.win);
}

void core::CamBegin(Camera2D &camera){
    Matrix proj = gmath::GetProjectionMatrix(state.win_width, state.win_height);
    Matrix view = gmath::GetViewMatrix(camera);
    Matrix model = Matrix::Identity();

    mkr::SetUniform(state.dshader.uproj, proj);
    mkr::SetUniform(state.dshader.uview, view);
    mkr::SetUniform(state.dshader.umodel, model);
}

void core::CamEnd(){
    Matrix view = Matrix::Identity();
    mkr::SetUniform(state.dshader.uview, view);
}

void core::ScreenClear(Color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

double LockCPU(){
    double currTime = glfwGetTime();
    double elapsed = currTime - core::state.lastTime;

    // debug print
    // printf("duration: %.3f | elapsed: %.3f\n", core::state.duration, elapsed);

    if (elapsed <= core::state.duration){
#ifdef _WIN32
        /*
            we do this to avoid in Windows
            cut by half the real fps target
        */
        glfwWaitEventsTimeout((core::state.duration - elapsed) / 15.6f);
#else
        glfwWaitEventsTimeout(core::state.duration - elapsed);
#endif
    }

    double newTime = glfwGetTime();
    core::state.delta = newTime - core::state.lastTime;
    core::state.lastTime = newTime;

    return newTime;
}

bool core::Loop(){
    LockCPU();
    glfwPollEvents();
    if (glfwWindowShouldClose(state.win)) return false;
    return true;
}

void core::Finish(){
    UnloadDefault();
    glfwDestroyWindow(state.win);
    glfwTerminate();
}
