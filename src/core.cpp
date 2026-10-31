#include "core.hpp"
#include "ios.hpp"
#include "pwra.h"
#include "mkr.hpp"
#include "reg.hpp"
#include "scene.hpp"
#include "reg.hpp"
#include <iostream>

constexpr const char* VERSION = "0.15.9";

static std::unique_ptr<Scene> currScene = nullptr;
static std::unique_ptr<Scene> nextScene = nullptr;

void engine::core::win_flag(flags flag){
    if (flag == flags::resizable) flags_active[0] = 1;
    if (flag == flags::maximized) flags_active[1] = 1;
    if (flag == flags::fullscreen) flags_active[2] = 1;
}

void engine::core::set_scene(std::unique_ptr<Scene> scene){
    nextScene = std::move(scene);
}

void engine::core::initial_scene(std::unique_ptr<Scene> initial){
    currScene = std::move(initial);
    currScene->Init();

    while(loop()) {
        if (!time::clock()) {
            continue;
        }

        time::count_fps();
        ios::InputUpdate();

        if (nextScene){
            if (currScene) currScene->Exit();
            currScene = std::move(nextScene);
            currScene->Init();
        }

        mkr::ScreenClear(Black);
        currScene->Update(time::get_delta());

        mkr::RenderBegin();
        currScene->Draw();
        mkr::RenderEnd();
    }

    finish();
}

void engine::core::main_window(int width, int height, const char *title){

    if (mkr::configInit()) {
        std::cout << "ACT ENGINE v" << VERSION << "\n";
    }

    wmain.win_width = width;
    wmain.win_height = height;
    bool win_started = mkr::startWindow(width, height, title);

    if (win_started){
        time::start_time();
        mkr::Initialize();
        mkr::setWindowIcon("eng/w_icon.png");
        pwra_start_audio();
        reg::init();
        std::cout << "[INFO] ENGINE INITIALIZED\n";
    }
    else{
        std::cout << "[ERROR] ENGINE COULD NOT INITIALIZE\n";
    }
}

bool special_esc(){
	return glfwGetKey(wmain.main, 256) == 1;
}

bool engine::core::loop(){
    if (glfwWindowShouldClose(wmain.main) || special_esc()) {
        return false;
    }
    return true;
}

void engine::core::finish(){
    currScene.reset();
    nextScene.reset();
    reg::clear();
    pwra_end_audio();
    mkr::Shutdown();
}
