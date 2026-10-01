#include "time.hpp"
#include "glfw//glfw3.h"
#include <thread>
#include <chrono>

double Time::lastTime = 0.0;
float Time::delta = 0.0f;

int Time::fps = 0;
int Time::frameCount = 0;
float Time::fpsTimer = 0.0f;

float Time::GetDelta(){
    return delta;
}

int Time::GetFrames() {
    return fps;
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
        // WaitFor(FPS_TARGET - elapsed - 0.001);
        WaitTime();
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
