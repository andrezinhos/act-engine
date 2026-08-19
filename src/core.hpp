#pragma once

#include <cstdbool>
#include "glad.h"
#include "glfw/glfw3.h"
#include "utils.hpp"

enum Flags{
    VSYNC,
    RESIZABLE,
    MAXIMIZED,
};

struct CoreState{
    GLFWwindow* win = nullptr;
    int win_width, win_height;
    int flags_active[3];
    Shader dshader;
    Mesh dmesh;
    Texture dtex;

    double lastTime = 0.0f;
    float targetfps;
    float duration;
};

class core {
private:
    static void init();
    static void LoadDefault();
    static void UnloadDefault();
public:
    static CoreState state;

    static void WindowFlag(Flags flag);
    static void TargetFPS(double fps);
    static void MainWindow(int width, int height, const char* title);

    static void DrawBegin();
    static void DrawEnd();
    static void CamBegin(Camera2D& cam);
    static void CamEnd();

    static void ScreenClear(Color color);
    static bool Loop();
    static void Finish();
};
