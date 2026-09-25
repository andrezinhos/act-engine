#include "core.hpp"
#include "ios.hpp"
#include "pwra.h"
#include "mkgl.hpp"
#include "scene.hpp"
#include "stack.hpp"
#include <thread>
#include <chrono>

constexpr cstr VERSION = "0.14.12";
Time core::time = {};
std::unique_ptr<Scene> core::currScene = nullptr;
std::unique_ptr<Scene> core::nextScene = nullptr;

void core::WindowFlag(Flags flag){
    if (flag == RESIZABLE) flags_active[0] = 1;
    if (flag == MAXIMIZED) flags_active[1] = 1;
    if (flag == FULLSCREEN) flags_active[2] = 1;
}

void core::init(){
    bool has_init = glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (has_init) printf("ACT ENGINE v%s\n", VERSION);
}

void core::setScene(std::unique_ptr<Scene> scene){
    nextScene = std::move(scene);
}

void core::InitialScene(std::unique_ptr<Scene> initial){
    currScene = std::move(initial);
    currScene->Init();

    while(Loop()) {
        if (!time.Clock()) continue;
        time.CountFps();
        ios::InputUpdate();

        if (nextScene){
            if (currScene) currScene->Exit();
            currScene = std::move(nextScene);
            currScene->Init();
        }

        mkr::ScreenClear(Black);
        currScene->Update(time.delta);

        mkr::RenderBegin();
        currScene->Draw();
        mkr::RenderEnd();
    }

    Finish();
}

void core::MainWindow(int width, int height, const char *title){
    core::init();
    wmain.win_width = width;
    wmain.win_height = height;
    bool win_started = mkr::startWindow(width, height, title);

    if (win_started){
        mkr::Initialize();
        time.lastTime = glfwGetTime();
        mkr::setWindowIcon("eng/w_icon.png");
        start_audio();
        printf("[INFO] ENGINE INITIALIZED\n");
    } else printf("[ERROR] ENGINE COULD NOT INITIALIZE");
}

bool special_esc(){
	return glfwGetKey(wmain.main, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

bool core::Loop(){
    if (glfwWindowShouldClose(wmain.main) ||
        special_esc()) return false;
    return true;
}

void core::Finish(){
    stack::UnloadAll();
    currScene.reset();
    nextScene.reset();
    end_audio();
    mkr::Shutdown();
}

float core::GetDelta(){
    return time.delta;
}

int core::GetFPS() {
    return time.fps;
}

void WaitTime(){
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1)
    );
}

void WaitFor(double value){
    std::this_thread::sleep_for(
        std::chrono::duration<double>(value)
    );
}

bool Time::Clock(){
    double newTime = glfwGetTime();
    double elapsed = newTime - lastTime;
    if (elapsed < 0.001) {
        WaitFor(FPS_TARGET - elapsed - 0.001);
        return false;
    }

    delta = elapsed;
    lastTime = newTime;
    if (delta > 0.1) delta = 0.1;
    return true;
}

void Time::CountFps(){
    frameCount += 1;
    fpsTimer += delta;

    if (fpsTimer >= 1.0f){
        fps = frameCount;
        frameCount = 0;
        fpsTimer -= 1.0f;
    }
}
