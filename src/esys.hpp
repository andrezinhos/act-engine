#pragma once
#include "pwra.h"
#include "mkr.h"
#include <cstdint>

class sound{
    Sfx* data;
    int id;
public:
    sound(std::string_view path);
    void play();
    void pitch(double amount);
};

class music{
    Stream* data;
    int id;
public:
    music(std::string_view path);
    void play();
    void stop();
    void pause();
    void resume();
};

struct entity{
    uint32_t id;
    bool active;
};

namespace esys{
    int create_ent();
};
