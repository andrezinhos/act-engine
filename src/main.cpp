#include "core.hpp"
#include "mkr.hpp"
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

	    Sprite sprite;

	    Image image = mkr::LoadImage("assets/sprites/smile.png");
	    sprite.texture = mkr::LoadTexture(image, LINEAR);

	    Image image2 = mkr::LoadImage("assets/sprites/neutral.png");
	    Texture tex2 = mkr::LoadTexture(image2, LINEAR);

	    sprite.texture.width = 100;
	    sprite.texture.height = 100;

	    float posX = (core::GetWindowWidth()/2) - (sprite.texture.width/2);
	    float posY = (core::GetWindowHeight()/2) - (sprite.texture.height/2);

	    sprite.position = {posX, posY};

	    Camera2D cam = {Vec2::Zero(), 0.0f, 1.0f};
	    while (core::Loop()) {
	        core::ScreenClear(Black);

			float speed = 500.0f;
	        if (ios::KeyDown(Keys::W)) sprite.position.y -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::S)) sprite.position.y += speed * core::GetDelta();
	        if (ios::KeyDown(Keys::A)) sprite.position.x -= speed * core::GetDelta();
	        if (ios::KeyDown(Keys::D)) sprite.position.x += speed * core::GetDelta();

			core::Follow(cam, sprite);

	        core::DrawBegin();
	        core::CamBegin(cam);

	        // mkr::DrawTexture(&tex2, Vec2::Zero(), {500, 500}, White);
	        mkr::DrawSprite(sprite, 1.0f, White);

	        core::CamEnd();
	        core::DrawEnd();
	    }

	    mkr::UnloadTexture(sprite.texture);
	    mkr::UnloadTexture(tex2);

	    core::Finish();
    }
    return 0;
}
