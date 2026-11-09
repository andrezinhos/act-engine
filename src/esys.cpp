#include "esys.hpp"
#include "mkr.h"
#include "pwra.h"
#include "core.hpp"

void Sound::load(const char* path){
    id = engine::reg::sfx_register(path);
    data = engine::reg::get_sfx(id);
}

void Sound::play(){
    if (data) {
        pwra_play_sfx(data);
    }
}

void Sound::pitch(double amount){
    if (data) {
        pwra_set_pitch(data, amount);
    }
}

void Music::load(const char* path){
    id = engine::reg::stream_register(path);
    data = engine::reg::get_music(id);
}

void Music::play(){
    if (data) {
        pwra_play_stream(data);
    }
}

void Music::stop(){
    if (data) {
        pwra_stop_stream(data);
    }
}

void Music::pause(){
    if (data) {
        pwra_pause_stream(data);
    }
}

void Music::resume(){
    if (data) {
        pwra_resume_stream(data);
    }
}

void Rect::pos(float x, float y){
    source.x = x;
    source.y = y;
}

void Rect::size(int x, int y){
    source.width = x;
    source.height = y;
}

void Rect::draw(color color){
    mk_render_rectangle(
        {(float)source.x, (float)source.y},
        {(float)source.width, (float)source.height},
        color
    );
}
