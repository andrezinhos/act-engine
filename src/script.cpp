#include "gfx.hpp"
#include "ios.hpp"
#include "core.hpp"
#include "sol.hpp"
#include "utils.hpp"
#include <string>

sol::state ios::script;

void ios::start_types(){
    script.new_enum("flags",
        "vsync", Flags::VSYNC,
        "resizable", Flags::RESIZABLE
    );

    script.new_usertype<Vec2>("vec2",
        sol::constructors<Vec2(float, float)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        "zero", &Vec2::Zero
    );

    script.new_usertype<Camera2D>("cam_2d",
        sol::constructors<Camera2D(Vec2, float, float)>(),
        "position", &Camera2D::position,
        "rotation", &Camera2D::rotation,
        "zoom", &Camera2D::zoom
    );

    script.new_usertype<Texture>("tex_2d",
        sol::meta_function::garbage_collect, sol::destructor([](Texture& tex){
            gfx::UnloadTexture(tex);
        }),
        "width", &Texture::width,
        "height", &Texture::height
    );

    script.new_usertype<Color>("Color",
        "r", &Color::r,
        "g", &Color::g,
        "b", &Color::b
    );

    script.new_enum("key",
        "w", Keys::W,
        "a", Keys::A,
        "s", Keys::S,
        "d", Keys::D
    );

    sol::table color_table = script.create_table();
    color_table["red"] = Red;
    color_table["green"] = Green;
    color_table["blue"] = Blue;
    color_table["white"] = White;
    color_table["black"] = Black;
    script["color"] = color_table;
}
void ios::init_script(){
    script.open_libraries(sol::lib::base, sol::lib::package);

    start_types();
}

void ios::start_funcs(){
    /* CORE FUNCS */
    sol::table core_table = script.create_table();

    core_table["win_flag"] = [](Flags flag){
        core::WindowFlag(flag);
    };

    core_table["start"] = [](int width, int height, const std::string& title){
        core::MainWindow(width, height, title.c_str());
    };

    core_table["clear"] = [](Color color){
        core::ScreenClear(color);
    };

    core_table["loop"] = core::Loop;
    core_table["stop"] = core::Finish;

    core_table["begin_draw"] = core::DrawBegin;
    core_table["end_draw"] = core::DrawEnd;

    core_table["cam_begin"] = [](Camera2D& cam){
        core::CamBegin(cam);
    };

    core_table["cam_end"] = core::CamEnd;

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

    ios_table["load_tex"] = [](const std::string& path) -> Texture {
        Image image = gfx::LoadImage(path.c_str());
        return gfx::LoadTexture(image);
    };

    script["ios"] = ios_table;

    /* RENDER FUNCS */

    sol::table gfx_table = script.create_table();

    gfx_table["draw_tex"] = [](Texture* tex, Vec2 position, Vec2 size){
        gfx::DrawTexture(tex, position, size, White);
    };

    script["render"] = gfx_table;
}

void ios::LoadScriptFuncs(){
    start_funcs();

    script.script_file("init.lua");
}
