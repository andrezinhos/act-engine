#include "core.hpp"
#include "mkr.h"
#include "pwra.h"
#include "glfw/glfw3.h"
#include <iostream>

constexpr std::string_view VERSION = "0.16.1";

static std::unique_ptr<engine::scene> currScene = nullptr;
static std::unique_ptr<engine::scene> nextScene = nullptr;

window engine::core::mainw = {};

void engine::core::win_flag(flags flag){
    if (flag == flags::resizable) mainw.flags[0] = 1;
    if (flag == flags::maximized) mainw.flags[1] = 1;
    if (flag == flags::fullscreen) mainw.flags[2] = 1;
}

void engine::core::set_scene(std::unique_ptr<scene> scene){
    nextScene = std::move(scene);
}

void engine::core::initial_scene(std::unique_ptr<scene> initial){
    currScene = std::move(initial);
    currScene->init();

    while(loop()) {
        if (!time::clock()) {
            continue;
        }

        time::count_fps();
        io::input_update();

        if (nextScene){
            if (currScene) currScene->exit();
            currScene = std::move(nextScene);
            currScene->init();
        }

        mk_clear_screen(Black);
        currScene->update(time::get_delta());

        mk_render_begin();
        mk_ortho_start({{0.0f, 0.0f}, 0.0f, 1.0f}, mainw.width, mainw.height);
        currScene->draw();
        mk_render_end();
        mainw.swap();
    }

    finish();
}

void engine::core::main_window(int width, int height, std::string_view title){
    if (mainw.config()){
        std::cout << "ACT ENGINE v" << VERSION << "\n";
    }

    if (mainw.flags[0] == 1) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }

    bool win_start = mainw.init(width, height, title);

    if (mainw.flags[1] == 1) {
        glfwMaximizeWindow(mainw.data);
    }

    if (win_start){
        mainw.pos_center();
        mainw.context();

        time::start_time();
        mk_init();
        mainw.icon("eng/w_icon.png");
        pwra_start_audio();
        reg::init();
        std::cout << "[INFO] ENGINE INITIALIZED\n";
    }
    else{
        std::cout << "[ERROR] ENGINE COULD NOT INITIALIZE\n";
    }
}

bool special_esc(){
	return glfwGetKey(engine::core::mainw.data, 256) == 1;
}

bool engine::core::loop(){
    if (mainw.should_close() || special_esc()){
        return false;
    }

    return true;
}

void engine::core::finish(){
    currScene.reset();
    nextScene.reset();
    reg::clear();
    pwra_end_audio();
    mainw.destroy();
    mk_shutdown();
}
