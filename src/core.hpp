#pragma once
#include "gmath.hpp"
#include <cstdbool>
#include <cstdint>
#include "utils.hpp"
#include "mkr.hpp"

enum Flags{
    VSYNC,
    RESIZABLE,
    MAXIMIZED,
    FULLSCREEN,
};

struct Window{
	GLFWmonitor* moni = nullptr;
	const GLFWvidmode* mode = nullptr;
    GLFWwindow* main = nullptr;
    int win_width, win_height;
};

struct CoreState{
    Window window;
    int flags_active[4];
    Shader dshader;
    Mesh dmesh;
    Texture dtex;

    uint32_t tvao;
    uint32_t tvbo;

    double lastTime = 0.0f;
    float delta;
    double targetfps;
    double duration;
};

class core {
private:
    static void init();
    static void LoadDefault();
    static void UnloadDefault();
public:
    static CoreState state;

    static float GetDelta();
    static int GetWindowWidth();
    static int GetWindowHeight();

    static void WindowFlag(Flags flag);
    static void TargetFPS(double fps);
    static void MainWindow(int width, int height, const char* title);

    static void DrawBegin();
    static void DrawEnd();
    static void CamBegin(Camera2D& cam);
    static void CamBegin(Camera3D& cam);
    static void CamEnd();

    static void Follow(Camera2D& cam, Sprite& sprite);

    static void ScreenClear(Color color);
    static bool Loop();
    static void Finish();
};
