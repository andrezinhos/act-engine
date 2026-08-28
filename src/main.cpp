#include "core.hpp"
#include "ios.hpp"

int main(){

	bool debug_mode = true;

	if (!debug_mode){
    	ios::init_script();
    	ios::LoadScriptFuncs();
	}
    // test code
	else {
		core::WindowFlag(RESIZABLE);
	    core::TargetFPS(60);
	    core::MainWindow(800, 600, "Window");

	    Texture texture = mkr::LoadTexture("assets/sprites/smile.png");
	    Texture tex2 = mkr::LoadTexture("assets/sprites/neutral.png");

	    texture.width = 100;
	    texture.height = 100;

	    float posX = (core::GetWindowWidth()/2) - (texture.width/2);
	    float posY = (core::GetWindowHeight()/2) - (texture.height/2);

	    Vec2 pos = {posX, posY};

	    Camera2D cam = {Vec2::Zero(), 0.0f, 1.0f};
	    while (core::Loop()) {
	        core::ScreenClear(Black);

			float speed = 500.0f;
	        if (ios::KeyDown(Keys::W)) pos.y -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::S)) pos.y += speed * core::GetDelta();
	        if (ios::KeyDown(Keys::A)) pos.x -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::D)) pos.x += speed * core::GetDelta();

			// core::Follow(cam, sprite);

	        mkr::RenderBegin();
	        mkr::CameraBegin(cam);

	        mkr::RenderTexture(&tex2, Vec2::Zero(), {500, 500}, White);

	        mkr::CameraEnd();
	        mkr::RenderEnd();
	    }

	    mkr::UnloadTexture(texture);
	    mkr::UnloadTexture(tex2);

	    core::Finish();
    }
    return 0;
}
