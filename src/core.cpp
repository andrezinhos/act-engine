#include "core.hpp"
#include "gmath.hpp"
#include "bgp.hpp"
#include <cstdio>
#include <chrono>
#include <thread>

constexpr const char* VERSION = "0.6.0";
CoreState core::state;

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
    if (bgp::batch.vao != 0 && bgp::batch.vbo != 0 && bgp::batch.ebo != 0){
        glDeleteVertexArrays(1, &bgp::batch.vao);
        glDeleteBuffers(1, &bgp::batch.vbo);
        glDeleteBuffers(1, &bgp::batch.ebo);
    }
}

void core::LoadDefault(){
    state.dshader = bgp::DefaultShader();
    state.dmesh = bgp::DefaultQuad();
    state.dtex = bgp::DefaultTexture();
    bgp::DefaultBatch();
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (has_init) printf("GGE v%s\n", VERSION);
}

void core::TargetFPS(double fps){
    state.targetfps = fps;
    state.duration = 1 / state.targetfps;
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

    /* for some reason, this makes the camera useless, so for now is off */
    // glfwSetFramebufferSizeCallback(win, frameCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load GLAD");
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
        printf("[INFO] ENGINE INITIALIZED\n");
    }
}

void core::DrawBegin(){
    glUseProgram(state.dshader.id);
    glUniform1i(state.dshader.utex, 0);
}

void core::DrawEnd(){
    bgp::Flush();
}

void core::CamBegin(Camera2D &camera){
    Matrix proj = bgp::GetProjectionMatrix(state.win_width, state.win_height);
    Matrix view = bgp::GetViewMatrix(camera);

    bgp::SetUniform(state.dshader.uproj, proj);
    bgp::SetUniform(state.dshader.uview, view);
}

void core::CamEnd(){
    Matrix view = Matrix::Identity();
    bgp::SetUniform(state.dshader.uview, view);
}

void core::ScreenClear(Color color){
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

double LockCPU(){
    double currTime = glfwGetTime();
    double elapsed = currTime - core::state.lastTime;

    if (elapsed < core::state.duration){
        std::this_thread::sleep_for(
            std::chrono::duration<double>(core::state.duration - elapsed)
        );
    }

    return glfwGetTime();
}

bool core::Loop(){
    core::state.lastTime = LockCPU();
    if (state.flags_active[0] == 1) glfwSwapInterval(1);
    glfwPollEvents();
    glfwSwapBuffers(core::state.win);
    if (glfwWindowShouldClose(state.win)) return false;
    return true;
}

void core::Finish(){
    UnloadDefault();
    glfwDestroyWindow(state.win);
    glfwTerminate();
}
