#include "core.hpp"
#include "ios.hpp"
#include "pwra.h"
#include "mkr.hpp"
#include "scene.hpp"
#include "time.hpp"
#include "stack.hpp"

constexpr cstr VERSION = "0.15.5";

std::unique_ptr<Scene> core::currScene = nullptr;
std::unique_ptr<Scene> core::nextScene = nullptr;

void core::WindowFlag(Flags flag){
    if (flag == RESIZABLE) flags_active[0] = 1;
    if (flag == MAXIMIZED) flags_active[1] = 1;
    if (flag == FULLSCREEN) flags_active[2] = 1;
}

void core::setScene(std::unique_ptr<Scene> scene){
    nextScene = std::move(scene);
}

void core::InitialScene(std::unique_ptr<Scene> initial){
    currScene = std::move(initial);
    currScene->Init();

    while(Loop()) {
        if (!Time::Clock()) continue;
        Time::CountFps();
        ios::InputUpdate();

        if (nextScene){
            if (currScene) currScene->Exit();
            currScene = std::move(nextScene);
            currScene->Init();
        }

        mkr::ScreenClear(Black);
        currScene->Update(Time::GetDelta());

        mkr::RenderBegin();
        currScene->Draw();
        mkr::RenderEnd();
    }

    Finish();
}

void core::MainWindow(int width, int height, const char *title){

    if (mkr::configInit()) printf("ACT ENGINE v%s\n", VERSION);

    wmain.win_width = width;
    wmain.win_height = height;
    bool win_started = mkr::startWindow(width, height, title);

    if (win_started){
        mkr::Initialize();
        mkr::setWindowIcon("eng/w_icon.png");
        pwra_start_audio();
        reg::init();
        printf("[INFO] ENGINE INITIALIZED\n");
    }
    else printf("[ERROR] ENGINE COULD NOT INITIALIZE\n");
}

bool special_esc(){
	return glfwGetKey(wmain.main, 256) == 1;
}

bool core::Loop(){
    if (glfwWindowShouldClose(wmain.main) ||
        special_esc()) return false;
    return true;
}

void core::Finish(){
    reg::clear();
    currScene.reset();
    nextScene.reset();
    pwra_end_audio();
    mkr::Shutdown();
}
