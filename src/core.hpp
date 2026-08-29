#pragma once
#include "gmath.hpp"
#include <cstdbool>
#include <cstdint>
#include "mkr/mkr.hpp"
#include "glfw/glfw3.h"
#include "utils.hpp"

enum Flags{
    VSYNC,
    RESIZABLE,
    MAXIMIZED,
    FULLSCREEN,
};

struct Time{
    double lastTime = 0.0f;
    float delta;
    double targetfps;
    double duration;
};

class core {
private:
    static void init();
public:
    static Time time;
    static float GetDelta();
    static int GetWindowWidth();
    static int GetWindowHeight();

    static void WindowFlag(Flags flag);
    static void TargetFPS(double fps);
    static void MainWindow(int width, int height, const char* title);

    // static void Follow(Camera2D& cam, Sprite& sprite);


    static bool Loop();
    static void Finish();
};
