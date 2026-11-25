#include "esys.hpp"
#include "mkr.h"
#include "pwra.h"
#include "core.hpp"
#include <vector>

bool sound::load(std::string_view path){
    id = engine::reg::sfx_register(path);

    if (id == -1){
        std::cout << "[ERROR] sound file not founded\n";
        return false;
    }

    data = engine::reg::get_sfx(id);
    return true;
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

bool music::load(std::string_view path){
    id = engine::reg::stream_register(path);

    if (id == -1){
        std::cout << "[ERROR] music file not founded\n";
        return false;
    }

    data = engine::reg::get_music(id);

    return true;
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

transform_2d::transform_2d(){
    position = bml_vec2_zero();
    scale = bml_vec2_zero();
}

constexpr int MAX_ENTS = 50;
static std::vector<entity> ents = {};
static std::vector<transform_2d> entpos_2d = {};
static std::vector<int> enttex_2d = {};
static std::vector<sheet> entanim_2d = {};

void esys::init(){
    ents.reserve(MAX_ENTS);
    entpos_2d.reserve(MAX_ENTS);
    enttex_2d.resize(MAX_ENTS);
    entanim_2d.resize(MAX_ENTS);
}

void esys::aclear(){
    ents.clear();
    entpos_2d.clear();
    enttex_2d.clear();

    for (auto& a : entanim_2d){
        val_arenaf_free(&a.frames);
    }
    entanim_2d.clear();
}

uint32_t esys::create_ent(){
    entity ent;

    ents.push_back(ent);
    ents.back().id = ents.size() - 1;
    ents.back().active = true;

    transform_2d t;
    entpos_2d.push_back(t);

    return ents.back().id;
}

void esys::ortho::ent_pos(uint32_t id, vec2 position){
    entpos_2d[id].position = position;
}

void esys::ortho::ent_scale(uint32_t id, vec2 scale){
    entpos_2d[id].scale = scale;
}

void esys::ortho::ent_load_tex(uint32_t id, std::string_view path){
    enttex_2d[id] = engine::reg::tex_register(path);
}

void esys::ortho::ent_render(uint32_t id){
    texture* tex = engine::reg::get_tex(enttex_2d[id]);
    mk_render_tex(tex, entpos_2d[id].position, entpos_2d[id].scale);
}

void esys::animation::new_animation(uint32_t id, int frames, float frames_duration){
    sheet anim = {};

    if (!val_arenaf_init(&anim.frames, frames)){
        std::cout << "failed to create animation\n";
        return;
    }

    val_arenaf_duration(&anim.frames, frames_duration);
    entanim_2d[id] = std::move(anim);
}

void esys::animation::add_frame(uint32_t id, double x, double y, double w, double h){
    val_arenaf_add_frame(
    	&entanim_2d[id].frames,
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(w),
        static_cast<float>(h)
    );
}

void esys::animation::play(uint32_t id, bool loop){
    sheet* anim = &entanim_2d[id];

    if (anim->end && !loop) return;

    anim->counter += engine::time::get_delta();

    if (anim->counter >= anim->frames.duration){
        anim->counter -= anim->frames.duration;
        anim->frames.currf++;

        if (anim->frames.currf >= (int)anim->frames.cap){
            if (loop){
                anim->frames.currf = 0;
            }
            else {
                anim->end = true;
                anim->frames.currf = 0;
            }
        }
    }
}

void esys::animation::render_animation(uint32_t id){
    texture* tex = engine::reg::get_tex(enttex_2d[id]);
    sheet* anim = &entanim_2d[id];

    mk_render_tex_region(
        tex,
        anim->frames.data[anim->frames.currf],
        entpos_2d[id].position,
        entpos_2d[id].scale,
        White
    );
}
