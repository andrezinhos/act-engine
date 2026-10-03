#pragma once

constexpr double FPS_TARGET = 1.0 / 72.0;

class Time{
    static double lastTime;
    static float delta;

    static int fps;
    static int frameCount;
    static float fpsTimer;
public:
    static float GetDelta();
    static int GetFrames();

    static bool Clock();
    static void CountFps();
};
