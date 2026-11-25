#include "core.hpp"
#include "esys.hpp"
#include "script.hpp"
#include "pwra.h"
#define SOL_NO_EXCEPTIONS 1
#define SOL_NO_RTTI 1
#define SOL_NOEXCEPT_FUNCTION_TYPE 1
#include "sol.hpp"

sol::state state;
sol::table scene_table;

void lscene::init(){
    scene_table = sol::nil;
    auto result = state.do_file(file.data());
    scene_table = result;
    if (scene_table["init"].valid()) {
        scene_table["init"]();
    }
}

void lscene::update(float dt){
    if (scene_table["update"].valid()){
        scene_table["update"](dt);
    }
}

void lscene::draw(){
    if (scene_table["draw"].valid()){
        scene_table["draw"]();
    }
}

void lscene::exit(){
    if (scene_table["exit"].valid()){
        scene_table["exit"]();
    }
}

void engine::script::start_types(){
	state.new_enum("key",
		"w", engine::keys::W,
		"a", engine::keys::A,
		"s", engine::keys::S,
		"d", engine::keys::D,
		"q", engine::keys::Q,
		"e", engine::keys::E,

        "left", engine::keys::LEFT,
        "right", engine::keys::RIGHT,
        "up", engine::keys::UP,
        "down", engine::keys::DOWN,

        "f1", engine::keys::F1,
        "f2", engine::keys::F2,
        "f3", engine::keys::F3,
        "f4", engine::keys::F4,
        "f5", engine::keys::F5
	);

	state["resizable"] = engine::flags::resizable;
	state["max"] = engine::flags::maximized;
	state["full"] = engine::flags::fullscreen;

	state["normal"] = engine::cursor::normal;
	state["hidden"] = engine::cursor::hidden;
	state["disabled"] = engine::cursor::disabled;

	state["red"] = Red;
	state["green"] = Green;
	state["blue"] = Blue;
	state["white"] = White;
	state["black"] = Black;
}

void engine::script::load_conteiners(){
    state.new_usertype<vec2>("vec2",
        sol::no_constructor,
        "set", &bml_vec2_set,
        "x", &vec2::x,
        "y", &vec2::y,
        "zero", &bml_vec2_zero
    );

    state.new_usertype<vec3>("vec3",
        sol::no_constructor,
        "set", &bml_vec3_set,
        "x", &vec3::x,
        "y", &vec3::y,
        "z", &vec3::z,
        "zero", &bml_vec2_zero
    );

    state.new_usertype<entity>("entity",
        sol::no_constructor,
        "id", &entity::id,
        "active", &entity::active
    );

    state.new_usertype<sound>("sound",
        "load", &sound::load,
        "play", &sound::play
    );

    state.new_usertype<music>("music",
        "load", &music::load,
        "play", &music::play,
        "stop", &music::stop,
        "resume", &music::resume,
        "pause", &music::pause
    );
}

void engine::script::init_script(){
    state.open_libraries(sol::lib::base, sol::lib::package);
    start_types();
    load_conteiners();
}

void engine::script::start_funcs(){
    sol::table core_table = state.create_table();

    core_table["win_flag"] = [](flags flag){
        engine::core::mainw.flag(flag);
    };

    core_table["win_cursor"] = [](cursor c){
        engine::core::mainw.cursor(c);
    };

    core_table["init_win"] = &engine::core::main_window;

    core_table["initial_scene"] = [](const std::string& file){
        std::string path = "assets/scenes/"+file;
        engine::core::initial_scene(std::make_unique<lscene>(path));
    };

    core_table["new_scene"] = [](const std::string& file){
        std::string path = "assets/scenes/"+file;
        engine::core::set_scene(std::make_unique<lscene>(path));
    };

    core_table["debug"] = [](std::string_view log){
        std::cout << "[DEBUG] " << log << "\n";
    };

    core_table["error"] = [](std::string_view log){
        std::cout << "[ERROR] " << log << "\n";
    };

    core_table["fps"] = [](){
        std::string fps_str = "FPS: "+std::to_string(engine::time::frames());
        mk_render_text(fps_str.c_str(), {10, 30}, 32);
    };

    core_table["delta"] = &engine::time::get_delta;

    core_table["key_pressed"] = &engine::io::key_pressed;
    core_table["key_down"] = &engine::io::key_down;

    state["eng"] = core_table;

    sol::table sys_table = state.create_table();

    sys_table["new_ent"] = &esys::create_ent;
    sys_table["ent_pos"] = &esys::ortho::ent_pos;
    sys_table["ent_scale"] = &esys::ortho::ent_scale;

    sys_table["load_tex"] = &esys::ortho::ent_load_tex;
    sys_table["render"] = &esys::ortho::ent_render;

    sys_table["new_animation"] = &esys::animation::new_animation;
    sys_table["add_frame"] = &esys::animation::add_frame;
    sys_table["play_anim"] = &esys::animation::play;
    sys_table["render_anim"] = &esys::animation::render_animation;

    state["sys"] = sys_table;
}

void engine::script::load_script_file(){
    start_funcs();
    state.script_file("assets/scripts/init.lua");
}
