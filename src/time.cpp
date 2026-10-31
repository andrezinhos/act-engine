#include "core.hpp"
#include "glfw/glfw3.h"
#include <thread>
#include <chrono>

constexpr double FPS_TARGET = 1.0 / 72.0;

double engine::time::lastTime = 0.0;
float engine::time::delta = 0.0f;

int engine::time::fps = 0;
int engine::time::frameCount = 0;
float engine::time::fpsTimer = 0.0f;

void engine::time::start_time(){
    lastTime = glfwGetTime();
}

float engine::time::get_delta(){
    return delta;
}

int engine::time::get_frames() {
    return fps;
}

void engine::time::wait(){
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1)
    );
}

void engine::time::wait_for(double value){
    std::this_thread::sleep_for(
        std::chrono::duration<double>(value)
    );
}

bool engine::time::clock(){
    double newTime = glfwGetTime();
    double elapsed = newTime - lastTime;

    if (elapsed < FPS_TARGET){
        return false;
    }
    else if (elapsed >= FPS_TARGET) {
        wait();
    }

    delta = elapsed;
    lastTime = newTime;
    if (delta > 0.1) delta = 0.1;
    return true;
}

void engine::time::count_fps(){
    frameCount += 1;
    fpsTimer += delta;

    if (fpsTimer >= 1.0f){
        fps = frameCount;
        frameCount = 0;
        fpsTimer -= 1.0f;
    }
}
