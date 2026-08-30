#include "core.hpp"
#include "esys.hpp"
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

        Sprite neutral;
        neutral.load("assets/sprites/neutral.png");
        neutral.pos(0, 0);
        neutral.size(500, 500);

        Sprite smile;
        smile.load("assets/sprites/smile.png");
        smile.size(100, 100);

        float posX = ((float)core::GetWindowWidth()/2) - ((float)smile.tex.width/2);
        float posY = ((float)core::GetWindowHeight()/2) - ((float)smile.tex.height/2);
        
        smile.pos(posX, posY);

        Camera2D cam = {Vec2::Zero(), 0.0f, 1.0f};
        while (core::Loop()) {
            mkr::ScreenClear(Black);

    		float speed = 500.0f;
            if (ios::KeyDown(Keys::W)) smile.position.y -= speed * core::GetDelta();
            if (ios::KeyDown(Keys::S)) smile.position.y += speed * core::GetDelta();
            if (ios::KeyDown(Keys::A)) smile.position.x -= speed * core::GetDelta();
            if (ios::KeyDown(Keys::D)) smile.position.x += speed * core::GetDelta();

    		// core::Follow(cam, sprite);

            mkr::RenderBegin();
            mkr::CameraBegin(cam);

            esys::RenderSprite(neutral.id, neutral);
            esys::RenderSprite(smile.id, smile);

            mkr::CameraEnd();
            mkr::RenderEnd();
        }

        core::Finish();
    }
    return 0;
}
