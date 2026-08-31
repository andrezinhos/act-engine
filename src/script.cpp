#include "ios.hpp"
#include "core.hpp"
#include "esys.hpp"
#include <string>
#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

sol::state script;

void ios::start_types(){
    script.new_usertype<Vec2>("Vec2",
        sol::constructors<Vec2(float, float)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        "zero", &Vec2::Zero
    );

    script.new_usertype<Vec3>("Vec3",
        sol::constructors<Vec3(float, float, float)>(),
        "x", &Vec3::x,
        "y", &Vec3::y,
        "z", &Vec3::z,
        "zero", &Vec3::Zero
    );

    script.new_usertype<Camera2D>("Cam2D",
        sol::constructors<Camera2D(Vec2, float, float)>(),
        "position", &Camera2D::position,
        "rotation", &Camera2D::rotation,
        "zoom", &Camera2D::zoom
    );

    script.new_usertype<Texture>("Tex2D",
        "width", &Texture::width,
        "height", &Texture::height
    );

	script.new_enum("key",
		"w", Keys::W,
		"a", Keys::A,
		"s", Keys::S,
		"d", Keys::D
	);

    script["vsync"] = Flags::VSYNC;
    script["resizable"] = Flags::RESIZABLE;
    script["max"] = Flags::MAXIMIZED;
    script["full"] = Flags::FULLSCREEN;

    script["normal"] = Cursor::NORMAL;
    script["hidden"] = Cursor::HIDDEN;
    script["disabled"] = Cursor::DISABLED;

    script["red"] = Red;
    script["green"] = Green;
    script["blue"] = Blue;
    script["white"] = White;
    script["black"] = Black;
}

void ios::load_conteiners(){
    script.new_usertype<Sprite>("Sprite",
        "texture", &Sprite::tex,
        "position", &Sprite::position,
        "id", &Sprite::id,

        "load", &Sprite::load,
        "pos", &Sprite::pos,
        "size", &Sprite::size
    );
}

void ios::init_script(){
    script.open_libraries(sol::lib::base, sol::lib::package);
    start_types();
    load_conteiners();
}

void ios::start_funcs(){
    /* CORE FUNCS */
    sol::table core_table = script.create_table();

    core_table["win_flag"] = [](Flags flag){ core::WindowFlag(flag); };
    core_table["fps"] = [](double fps){ core::TargetFPS(fps); };
    core_table["delta"] = core::GetDelta;

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

    ios_table["key_down"] = [](Keys key){
    	return ios::KeyDown(key);
    };

    ios_table["key_pressed"] = [](Keys key){
    	return ios::KeyPressed(key);
    };

    script["ios"] = ios_table;

    /* RENDER FUNCS */

    sol::table mkr_table = script.create_table();

    mkr_table["win_width"] = mkr::GetWindowWidth;
    mkr_table["win_height"] = mkr::GetWindowHeight;

    mkr_table["begin_draw"] = mkr::RenderBegin;
    mkr_table["end_draw"] = mkr::RenderEnd;
    mkr_table["cam_begin"] = [](Camera2D& cam){ mkr::CameraBegin(cam); };
    mkr_table["cam_end"] = mkr::CameraEnd;

    mkr_table["cursor"] = [](Cursor cur) { mkr::setCursorMode(cur); };

    mkr_table["sprite"] = [](int id, Sprite& sprite){
        esys::RenderSprite(id, sprite);
    };

    script["render"] = mkr_table;
}

void ios::LoadScriptFuncs(){
    start_funcs();
    script.script_file("assets/scripts/init.lua");
}
