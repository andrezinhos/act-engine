#include "core.hpp"
#include "amk.hpp"
#include "ios.hpp"
#include "scene.hpp"
#include "stack.hpp"
#include "script.hpp"
#include <cstdio>
#include <memory>

constexpr const char* VERSION = "0.13.0";

Time core::time = {};
std::unique_ptr<Scene> core::currScene = nullptr;
std::unique_ptr<Scene> core::nextScene = nullptr;

void core::WindowFlag(Flags flag){
    switch(flag){
        case VSYNC: mkr::flags_active[0] = 1; break;
        case RESIZABLE: mkr::flags_active[1] = 1; break;
        case MAXIMIZED: mkr::flags_active[2] = 1; break;
        case FULLSCREEN: mkr::flags_active[3] = 1; break;
    }
}

void core::init(){
    bool has_init = glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (has_init) printf("ACT ENGINE v%s\n", VERSION);
}

void core::TargetFPS(double fps){
    time.targetfps = fps;
    time.duration = 1.0f / fps;
}

float core::GetDelta(){
    return time.delta;
}

void core::setScene(std::unique_ptr<Scene> scene){
    nextScene = std::move(scene);
}

void core::InitialScene(std::unique_ptr<Scene> initial){
    currScene = std::move(initial);
    currScene->Init();

    while(Loop()) {
        if (nextScene){
            if (currScene) currScene->Exit();
            currScene = std::move(nextScene);
            currScene->Init();
        }

        float dt = GetDelta();

        mkr::ScreenClear(Black);
        currScene->Update(dt);

        mkr::RenderBegin();
        currScene->Draw();
        mkr::RenderEnd();
    }

    Finish();
}

void core::MainWindow(int width, int height, const char *title){
    core::init();
    mkr::wmain.win_width = width;
    mkr::wmain.win_height = height;
    bool win_started = mkr::startWindow(width, height, title);

    if (win_started){
        mkr::Initialize();
        time.lastTime = glfwGetTime();
        mkr::setWindowIcon("assets/sprites/w_icon.png");
        amk::initAudioDevice();
        printf("[INFO] ENGINE INITIALIZED\n");
    } else printf("[ERROR] ENGINE COULD NOT INITIALIZE");
}

double LockCPU(){
    double currTime = glfwGetTime();
    double elapsed = currTime - core::time.lastTime;

    if (elapsed <= core::time.duration){
#ifdef _WIN32
        // we do this in Windows to avoid
        // cut by half the real fps target
        glfwWaitEventsTimeout((core::time.duration - elapsed) / 15.6f);
#else
        glfwWaitEventsTimeout(core::time.duration - elapsed);
#endif
    }

    double newTime = glfwGetTime();
    core::time.delta = newTime - core::time.lastTime;
    core::time.lastTime = newTime;

    return newTime;
}

bool special_esc(){
	return glfwGetKey(mkr::wmain.main, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

bool core::Loop(){
    LockCPU();
    ios::InputUpdate();
    glfwPollEvents();
    if (glfwWindowShouldClose(mkr::wmain.main) || special_esc()) return false;
    else return true;
}

void core::Finish(){
    script::state.collect_garbage();
    stack::UnloadAll();
    currScene.reset();
    nextScene.reset();
    amk::endAudioDevice();
    mkr::Shutdown();
}
