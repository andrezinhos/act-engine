#include "bgp.hpp"
#include "core.hpp"
#include "gfx.hpp"
#include "utils.hpp"
#include "script.hpp"

int main(){

    code::init_script();
    code::LoadScriptFuncs();
    // core::WindowFlag(Flags::RESIZABLE);
    // core::WindowFlag(Flags::VSYNC);
    // core::TargetFPS(60);

    // core::MainWindow(800, 600, "Hello");
    // Image image = gfx::LoadImage("smile.png");
    // Texture tex = gfx::LoadTexture(image);

    // Camera2D cam(Vec2::Zero(), 0.0f, 1.0f);
    // while(core::Loop()){
    //     core::ScreenClear(Black);

    //     core::DrawBegin();
    //     core::CamBegin(cam);

    //     gfx::DrawTexture(&tex, {100, 100}, {200, 200}, White);

    //     core::CamEnd();
    //     core::DrawEnd();
    // }

    // gfx::UnloadTexture(tex);
    // core::Finish();
    return 0;
}
