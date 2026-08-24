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
		core::WindowFlag(RESIZABLE);
	    core::TargetFPS(60);
	    core::MainWindow(800, 600, "Window");

	    Image image = mkr::LoadImage("assets/sprites/smile.png");
	    Texture tex = mkr::LoadTexture(image, LINEAR);

	    Image image2 = mkr::LoadImage("assets/sprites/neutral.png");
	    Texture tex2 = mkr::LoadTexture(image2, LINEAR);

	    tex.width = 100;
	    tex.height = 100;

	    float posX = (core::GetWindowWidth()/2) - (tex.width/2);
	    float posY = (core::GetWindowHeight()/2) - (tex.height/2);

	    printf("%.2f\n", posX);
	    printf("%.2f\n", posY);
	    
	    Vec2 pos = {posX, posY};

	    Camera2D cam = {Vec2::Zero(), 0.0f, 1.0f};
	    while (core::Loop()) {
	        core::ScreenClear(Black);

			float speed = 500.0f;
	        if (ios::KeyDown(Keys::W)) pos.y -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::S)) pos.y += speed * core::GetDelta();
	        if (ios::KeyDown(Keys::A)) pos.x -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::D)) pos.x += speed * core::GetDelta();

			cam.position = {
				pos.x - (core::GetWindowWidth() / 2) + (tex.width/2),
				pos.y - (core::GetWindowHeight() / 2) + (tex.height/2)
			};
			
	        core::DrawBegin();
	        core::CamBegin(cam);

	        mkr::DrawTexture(&tex2, Vec2::Zero(), {500, 500}, White);
	        mkr::DrawTexture(&tex, pos, {(float)tex.width, (float)tex.height}, White);

	        core::CamEnd();
	        core::DrawEnd();
	    }

	    mkr::UnloadTexture(tex);
	    mkr::UnloadTexture(tex2);

	    core::Finish();
    }
    return 0;
}
