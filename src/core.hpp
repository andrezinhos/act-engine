#pragma once
#include <cstdio>
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
};

namespace core {
    inline double lastTime = 0.0f;
    inline float targetfps;
    inline float duration;
    inline CoreState state;

    void LoadDefault();
    void UnloadDefault();
    void WindowFlag(Flags flag);
    void init();
    void TargetFPS(double fps);
    void MainWindow(int width, int height, const char* title);

    void DrawBegin();
    void DrawEnd();
    void CamBegin(Camera2D& cam);
    void CamEnd();

    void ScreenClear(Color color);
    bool Loop();
    void Finish();
};
