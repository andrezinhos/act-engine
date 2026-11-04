#pragma once
#include "scene.hpp"
#include <memory>
#include <string_view>
#include <cstdint>

namespace engine {
    enum class flags{
        resizable,
        maximized,
        fullscreen,
    };

    enum class keys {
        W = 87,
        A = 65,
        S = 83,
        D = 68,

        Q = 81,
        E = 69,

        LEFT = 263,
        RIGHT = 262,
        UP = 265,
        DOWN = 264,

        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
    };


    class image{
    private:
        uint8_t* _data = nullptr;
        int _width = 0;
        int _height = 0;
        int _ch = 0;
    public:
        image() = default;
        ~image();

        image(const image&) = delete;
        image& operator=(const image&) = delete;

        image(image&&) = delete;
        image& operator=(image&&) = delete;

        bool load(std::string_view);
        int width() const;
        int height() const;
        int channels() const;
        uint8_t* data() const;
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

    class io{
    public:
        static uint8_t* load_bytes(std::string_view path, size_t& size);

    	static void input_update();
        static bool key_down(engine::keys key);
        static bool key_pressed(engine::keys key);
    };
};
