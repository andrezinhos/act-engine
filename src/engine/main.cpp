#include "core.hpp"
#include "esys.hpp"
#include "gmath.hpp"
#include "ios.hpp"
#include "script.hpp"
#include <memory>

class Menu : public Scene{
public:
    Sprite neutral;
    Camera2D cam;
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
};

void Menu::Init(){
    neutral.load("assets/sprites/neutral.png");
    neutral.pos(0, 0);
    neutral.size(100, 100);

    cam = {Vec2::Zero(), 0.0f, 1.0f};
}

void Menu::Update(float dt){

}

void Menu::Draw(){
    mkr::CameraBegin(cam);
    mkr::RenderText("Hello World", {100, 100}, 100, White);
    mkr::CameraEnd();
}

int main(){
	bool debug_mode = true;

    if (!debug_mode){
       	script::init_script();
       	script::load_script_file();
    }
    // test code
    else {
    	core::WindowFlag(RESIZABLE);
        core::TargetFPS(60);
        core::MainWindow(800, 600, "Window");
        core::InitialScene(std::make_unique<Menu>());
    }

    return 0;
}
