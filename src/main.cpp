#include "core.hpp"
#include "mkr.hpp"
#include "ios.hpp"

int main(){

	bool debug_mode = false;

	if (!debug_mode){
    	ios::init_script();
    	ios::LoadScriptFuncs();
	}
    // test code

	else {
		core::WindowFlag(MAXIMIZED);
		core::WindowFlag(RESIZABLE);
	    core::TargetFPS(60);
	    core::MainWindow(800, 600, "Window");

	    Vec2 pos = {100, 100};
	    Camera2D cam = {Vec2::Zero(), 0.0f, 1.0f};

	    Image image = mkr::LoadImage("assets/sprites/smile.png");
	    Texture tex = mkr::LoadTexture(image, LINEAR);

	    Image image2 = mkr::LoadImage("assets/sprites/neutral.png");
	    Texture tex2 = mkr::LoadTexture(image2, LINEAR);

	    while (core::Loop()) {
	        core::ScreenClear(Black);

	        core::DrawBegin();
	        core::CamBegin(cam);

	        mkr::DrawTexture(&tex2, Vec2::Zero(), {500, 500}, White);
	        mkr::DrawTexture(&tex, pos, {100, 100}, White);

	        core::CamEnd();
	        core::DrawEnd();
	    }

	    mkr::UnloadTexture(tex);
	    mkr::UnloadTexture(tex2);

	    core::Finish();
    }
    return 0;
}
