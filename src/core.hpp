#pragma once
#include "scene.hpp"
#include <memory>

enum Flags{
    RESIZABLE,
    MAXIMIZED,
    FULLSCREEN,
};

class core {
private:
    static std::unique_ptr<Scene> currScene;
    static std::unique_ptr<Scene> nextScene;
public:
    static void setScene(std::unique_ptr<Scene> scene);
    static void InitialScene(std::unique_ptr<Scene> initial);

    static void WindowFlag(Flags flag);
    static void MainWindow(int width, int height, const char* title);

    static bool Loop();
    static void Finish();
};
