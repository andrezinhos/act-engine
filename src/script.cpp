#include "ios.hpp"
#include "core.hpp"
#include <string>
#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

sol::state script;

void ios::start_types(){
    script.new_enum("flags",
        "vsync", Flags::VSYNC,
        "resizable", Flags::RESIZABLE,
        "max", Flags::MAXIMIZED,
        "full", Flags::FULLSCREEN
    );

    script.new_usertype<Vec2>("vec2",
        sol::constructors<Vec2(float, float)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        "zero", &Vec2::Zero
    );

    script.new_usertype<Vec3>("vec3",
        sol::constructors<Vec3(float, float, float)>(),
        "x", &Vec3::x,
        "y", &Vec3::y,
        "z", &Vec3::z,
        "zero", &Vec3::Zero
    );

    script.new_usertype<Camera2D>("cam_2d",
        sol::constructors<Camera2D(Vec2, float, float)>(),
        "position", &Camera2D::position,
        "rotation", &Camera2D::rotation,
        "zoom", &Camera2D::zoom
    );

    script.new_usertype<Texture>("tex_2d",
        "width", &Texture::width,
        "height", &Texture::height
    );

	script.new_enum("key",
		"w", Keys::W,
		"a", Keys::A,
		"s", Keys::S,
		"d", Keys::D
	);

    script["red"] = Red;
    script["green"] = Green;
    script["blue"] = Blue;
    script["white"] = White;
    script["black"] = Black;
}

void ios::init_script(){
    script.open_libraries(sol::lib::base, sol::lib::package);
    start_types();
}

void ios::start_funcs(){
    /* CORE FUNCS */
    sol::table core_table = script.create_table();

    core_table["win_flag"] = [](Flags flag){ core::WindowFlag(flag); };
    core_table["fps"] = [](double fps){ core::TargetFPS(fps); };
    core_table["delta"] = core::GetDelta;

    core_table["win_width"] = core::GetWindowWidth;
    core_table["win_height"] = core::GetWindowHeight;

    core_table["start"] = [](int width, int height, const std::string& title){
        core::MainWindow(width, height, title.c_str());
    };

    core_table["clear"] = [](Color color){ mkr::ScreenClear(color); };

    core_table["loop"] = core::Loop;
    core_table["stop"] = core::Finish;

    script["eng"] = core_table;

    /* IO FUNCS */

    sol::table ios_table = script.create_table();

    ios_table["debug"] = [](const std::string& msg){
        printf("[DEBUG] %s\n", msg.c_str());
    };

    ios_table["err"] = [](const std::string& msg){
        printf("[ERROR] %s\n", msg.c_str());
        std::abort();
    };

    // ios_table["load_tex"] = [](const std::string& path) -> Texture {
    //     return ios::LoadTexture(path.c_str());
    // };

    ios_table["key_down"] = [](Keys key){
    	return ios::KeyDown(key);
    };

    ios_table["key_pressed"] = [](Keys key){
    	return ios::KeyPressed(key);
    };

    script["ios"] = ios_table;

    /* RENDER FUNCS */

    sol::table mkr_table = script.create_table();

    mkr_table["begin_draw"] = mkr::RenderBegin;
    mkr_table["end_draw"] = mkr::RenderEnd;
    mkr_table["cam_begin"] = [](Camera2D& cam){ mkr::CameraBegin(cam); };
    mkr_table["cam_end"] = mkr::CameraEnd;

    mkr_table["draw_tex"] = [](Texture* tex, Vec2 position, Vec2 size, Color color){
        mkr::RenderTexture(tex, position, size, color);
    };

    script["render"] = mkr_table;
}

void ios::LoadScriptFuncs(){
    start_funcs();
    script.script_file("assets/scripts/init.lua");
}
