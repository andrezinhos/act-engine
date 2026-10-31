#pragma once
#include "scene.hpp"
#include <memory>

namespace engine {
    enum class flags{
        resizable,
        maximized,
        fullscreen,
    };
};

namespace engine{
    class core {
    private:
        static bool loop();
        static void finish();
    public:
        static void set_scene(std::unique_ptr<Scene> scene);
        static void initial_scene(std::unique_ptr<Scene> initial);

        static void win_flag(flags flag);
        static void main_window(int width, int height, const char* title);
    };

    class time{
    private:
        static double lastTime;
        static float delta;

        static int fps;
        static int frameCount;
        static float fpsTimer;
    public:
        static void start_time();
        static float get_delta();
        static int get_frames();

        static void wait();
        static void wait_for(double value);

        static bool clock();
        static void count_fps();
    };
};
