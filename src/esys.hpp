#pragma once
#include "pwra.h"
#include "mkr.h"

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
