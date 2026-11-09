#include "core.hpp"
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

	state["red"] = Red;
	state["green"] = Green;
	state["blue"] = Blue;
	state["white"] = White;
	state["black"] = Black;
}

void engine::script::load_conteiners(){

}

void engine::script::init_script(){
    state.open_libraries(sol::lib::base, sol::lib::package);
    start_types();
    load_conteiners();
}

void engine::script::start_funcs(){
    sol::table core_table = state.create_table();

    core_table["win_flag"] = [](flags flag){
        engine::core::win_flag(flag);
    };

    core_table["init"] = [](int w, int h, std::string_view title){
        engine::core::main_window(w, h, title);
    };

    state["eng"] = core_table;
}

void engine::script::load_script_file(){
    start_funcs();
    state.script_file("assets/scripts/init.lua");
}
