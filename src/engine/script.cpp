#define SOL_ALL_SAFETIES_ON 1
#include "script.hpp"
#include <memory>

void script::start_types(){
    state.new_usertype<Vec2>("Vec2",
        sol::constructors<Vec2(float, float)>(),
        "x", &Vec2::x,
        "y", &Vec2::y,
        "zero", &Vec2::Zero
    );

    state.new_usertype<Vec3>("Vec3",
        sol::constructors<Vec3(float, float, float)>(),
        "x", &Vec3::x,
        "y", &Vec3::y,
        "z", &Vec3::z,
        "zero", &Vec3::Zero
    );

    state.new_usertype<Camera2D>("Cam2D",
        sol::constructors<Camera2D(Vec2, float, float)>(),
        "position", &Camera2D::position,
        "rotation", &Camera2D::rotation,
        "zoom", &Camera2D::zoom
    );

    state.new_usertype<Texture>("Tex2D",
        "width", &Texture::width,
        "height", &Texture::height
    );

	state.new_enum("key",
		"w", Keys::W,
		"a", Keys::A,
		"s", Keys::S,
		"d", Keys::D,
		"q", Keys::Q,
		"e", Keys::E
	);

	state["vsync"] = Flags::VSYNC;
	state["resizable"] = Flags::RESIZABLE;
	state["max"] = Flags::MAXIMIZED;
	state["full"] = Flags::FULLSCREEN;

	state["normal"] = Cursor::NORMAL;
	state["hidden"] = Cursor::HIDDEN;
	state["disabled"] = Cursor::DISABLED;

	state["red"] = Red;
	state["green"] = Green;
	state["blue"] = Blue;
	state["white"] = White;
	state["black"] = Black;
}

void script::load_conteiners(){
    state.new_usertype<Rect>("Rect",
        "width", &Rect::width,
        "height", &Rect::height,
        "position", &Rect::position,

        "pos", &Rect::pos,
        "size", &Rect::size,
        "draw", &Rect::draw
    );

    state.new_usertype<Sprite>("Sprite",
        "texture", &Sprite::tex,
        "position", &Sprite::position,
        "id", &Sprite::id,

        "load", &Sprite::load,
        "pos", &Sprite::pos,
        "size", &Sprite::size,
        "draw", &Sprite::draw
    );

    state.new_usertype<Sound>("Sound",
        "id", &Sound::id,

        "load", &Sound::load,
        "play", &Sound::play
    );

    state.new_usertype<Music>("Music",
        "id", &Music::id,

        "load", &Music::load,
        "play", &Music::play,
        "stop", &Music::stop,
        "pause", &Music::pause,
        "resume", &Music::resume
    );
}

void script::init_script(){
    state.open_libraries(sol::lib::base, sol::lib::package);
    start_types();
    load_conteiners();
}

void script::start_funcs(){
    /* CORE FUNCS */
    sol::table core_table = state.create_table();

    core_table["win_flag"] = [](Flags flag){ core::WindowFlag(flag); };
    core_table["fps"] = [](double fps){ core::TargetFPS(fps); };
    core_table["delta"] = core::GetDelta;

    core_table["initial_scene"] = [](const std::string& file){
        std::string path = "assets/scenes/"+file;
        core::InitialScene(std::make_unique<LuaScene>(path));
    };

    core_table["change_scene"] = [](const std::string& file){
        std::string path = "assets/scenes/"+file;
        core::setScene(std::make_unique<LuaScene>(path));
    };

    core_table["start"] = [](int width, int height, const std::string& title){
        core::MainWindow(width, height, title.c_str());
    };

    core_table["master_vol"] = [](double vol){ amk::MasterVolume(vol); };
    state["eng"] = core_table;

    /* IO FUNCS */

    sol::table ios_table = state.create_table();

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

    state["ios"] = ios_table;

    /* RENDER FUNCS */

    sol::table mkr_table = state.create_table();

    mkr_table["win_width"] = mkr::GetWindowWidth;
    mkr_table["win_height"] = mkr::GetWindowHeight;

    mkr_table["cam_begin"] = [](Camera2D& cam){ mkr::CameraBegin(cam); };
    mkr_table["cam_end"] = mkr::CameraEnd;

    mkr_table["cursor"] = [](Cursor cur) { mkr::setCursorMode(cur); };

    state["render"] = mkr_table;
}

void script::load_script_file(){
    start_funcs();
    state.script_file("assets/scripts/init.lua");
}
