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

struct Sfx;
struct Stream;
struct Texture;
struct Font;

namespace engine{
    namespace core {
        bool loop();
        void finish();
        void set_scene(std::unique_ptr<Scene> scene);
        void initial_scene(std::unique_ptr<Scene> initial);

        void win_flag(flags flag);
        void main_window(int width, int height, const char* title);
    };

    namespace time{
        void start_time();
        float get_delta();
        int get_frames();

        void wait();
        void wait_for(double value);

        bool clock();
        void count_fps();
    };

    namespace io{
        uint8_t* load_bytes(std::string_view path, size_t& size);

    	void input_update();
        bool key_down(engine::keys key);
        bool key_pressed(engine::keys key);
    };

    namespace reg{
        void init();

        int tex_register(std::string_view path);
        int sfx_register(std::string_view path);
        int stream_register(std::string_view path);
        int font_register(std::string_view path);

        Texture* get_tex(int id);
        Sfx* get_sfx(int id);
        Stream* get_music(int id);
        Font* get_font(int id);

        void clear_tex();
        void clear_sfx();
        void clear_stream();
        void clear_font();

        void clear();
    };

};
