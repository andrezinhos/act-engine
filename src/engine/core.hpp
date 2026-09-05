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
    double lastTime;
    float delta;
    double duration;

    int fps;
    int frameCount;
    double fpsTimer;
    
    double Clock(); 
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
