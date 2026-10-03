#include "core.hpp"
#include "esys.hpp"
#include "mkmath.hpp"
#include "script.hpp"
#include <memory>

class Menu : public Scene{
public:
    Sprite neutral;
    Camera2D cam;
    Anim2D animation;
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
};

void Menu::Init(){
    neutral.load("assets/sprites/neutral.png");
    neutral.pos(0, 0);
    neutral.size(100, 100);

    animation.load("assets/sprites/rgb.png");

    animation.set_frames({
        {0, 64, 64, 64},
        {0, 64, 64, 64},
        {0, 64, 64, 64},
        {0, 64, 64, 64},
    });

    cam = {Vec2Zero(), 0.0f, 1.0f};
}

void Menu::Update(float dt){

    // printf("DELTA: %.3f\n", dt);
    // printf("FPS: %d\n", core::GetFPS());
    // fps_string = "FPS: " + std::to_string(core::GetFPS());
}

void Menu::Draw(){
    // mkr::CameraBegin(cam);
    neutral.draw();
    // mktxt::RenderText(fps_string, {10, 50}, 50, White);
    // mkr::CameraEnd();
}

int main(){
	bool debug_mode = false;

    if (!debug_mode){
       	script::init_script();
       	script::load_script_file();
    }
    // test code
    else {
    	core::WindowFlag(RESIZABLE);
        core::MainWindow(800, 600, "Window");
        core::InitialScene(std::make_unique<Menu>());
    }

    return 0;
}
