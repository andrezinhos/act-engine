#include "core.hpp"
#include <chrono>
#include <thread>

Time core::time = {};

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

double Time::Clock(){
    double newTime = glfwGetTime();
    delta = static_cast<float>(newTime - lastTime);
    lastTime = newTime;
    if (delta > 0.1) delta = 0.1;

    frameCount++;
    fpsTimer += core::time.delta;

    if (fpsTimer >= 1.0) {
        fps = frameCount;
        frameCount = 0;
        fpsTimer = 0.0;
    }

    WaitTime();
    return newTime;
}
