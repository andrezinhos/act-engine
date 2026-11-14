#include "esys.hpp"
#include "mkr.h"
#include "pwra.h"
#include "core.hpp"
#include <vector>

sound::sound(std::string_view path){
    id = engine::reg::sfx_register(path);
    data = engine::reg::get_sfx(id);
}

void sound::play(){
    if (data){
        pwra_play_sfx(data);
    }
}

void sound::pitch(double amount){
    if (data){
        pwra_set_pitch(data, amount);
    }
}

music::music(std::string_view path){
    id = engine::reg::stream_register(path);
    data = engine::reg::get_music(id);
}

void music::play(){
    if (data){
        pwra_play_stream(data);
    }
}

void music::stop(){
    if (data){
        pwra_stop_stream(data);
    }
}

void music::pause(){
    if (data){
        pwra_pause_stream(data);
    }
}

void music::resume(){
    if (data){
        pwra_resume_stream(data);
    }
}

constexpr int MAX_ENTS = 50;
static std::vector<entity> ents = {};
