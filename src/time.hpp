#pragma once

class Time{
    static double lastTime;
    static float delta;

    static int fps;
    static int frameCount;
    static float fpsTimer;
public:
    static void StartTime();
    static float GetDelta();
    static int GetFrames();

    static void WaitTime();
    static void WaitFor(double value);

    static bool Clock();
    static void CountFps();
};
