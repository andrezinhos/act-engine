#pragma once
#include "gmath.hpp"
#include "mkr.hpp"
#include "scene.hpp"
#include <memory>

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
    static std::unique_ptr<Scene> currScene;
    static std::unique_ptr<Scene> nextScene;
public:
    static Time time;
    static float GetDelta();

    static void setScene(std::unique_ptr<Scene> scene);
    static void InitialScene(std::unique_ptr<Scene> initial);

    static void WindowFlag(Flags flag);
    static void TargetFPS(double fps);
    static void MainWindow(int width, int height, const char* title);

    // not implemented
    // static void Follow(Camera2D& cam, Sprite& sprite);

    static bool Loop();
    static void Finish();
};
