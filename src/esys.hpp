#pragma once
#include "pwra.h"
#include "mkr.h"
#include <cstdint>

class sound{
    Sfx* data = nullptr;
    int id = -1;
public:
    sound() = default;
    bool load(std::string_view path);
    void play();
    void pitch(double amount);
};

class music{
    Stream* data = nullptr;
    int id = -1;
public:
    music() = default;
    bool load(std::string_view path);
    void play();
    void stop();
    void pause();
    void resume();
};

class camera{
    camera_data_2d data;
public:
    camera(vec2 position, float rotation, float zoom);
    void reset();
    void use();
};

struct entity{
    uint32_t id;
    bool active;
};

struct transform_2d{
    vec2 position;
    vec2 scale;

    transform_2d();
};

struct sheet{
    arena_f frames;
    float counter;
    bool end;
};

namespace esys{
    void init();
    void aclear();
    uint32_t create_ent();

    namespace animation{
        void new_animation(uint32_t id, int frames, float frames_duration);
        void add_frame(uint32_t id, double x, double y, double w, double h);
        void play(uint32_t id, bool loop);
        void render_animation(uint32_t id);
    };

    namespace ortho{
        void ent_pos(uint32_t id, vec2 position);
        void ent_scale(uint32_t id, vec2 scale);

        void ent_load_tex(uint32_t id, std::string_view path);
        void move_x(uint32_t id, float x);
        void move_y(uint32_t id, float y);
        void ent_render(uint32_t id);
    }
};
