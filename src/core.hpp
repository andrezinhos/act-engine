#pragma once
#include "scene.hpp"
#include <memory>

static const double FPS_TARGET = 1.0 / 72.0;

enum Flags{
    RESIZABLE,
    MAXIMIZED,
    FULLSCREEN,
};

struct Time{
    double lastTime;
    float delta;

    int fps;
    int frameCount;
    float fpsTimer;
    bool Clock();
    void CountFps();
};

class core {
private:
    static void init();
    static std::unique_ptr<Scene> currScene;
    static std::unique_ptr<Scene> nextScene;
public:
    static Time time;
    static float GetDelta();
    static int GetFPS();

    static void setScene(std::unique_ptr<Scene> scene);
    static void InitialScene(std::unique_ptr<Scene> initial);

    static void WindowFlag(Flags flag);
    static void MainWindow(int width, int height, const char* title);

    static bool Loop();
    static void Finish();
};
