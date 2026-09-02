#include "core.hpp"
#include "esys.hpp"
#include "gmath.hpp"
#include "ios.hpp"
#include "script.hpp"
#include <memory>

// class Menu : public Scene{
// public:
//     Sprite neutral;
//     Camera2D cam;
//     Sound sound;
//     void Init() override;
//     void Update(float dt) override;
//     void Draw() override;
// };

// class Game : public Scene{
// public:
//     Sprite smile;
//     Camera2D cam;
//     void Init() override;
//     void Update(float dt) override;
//     void Draw() override;
// };

// void Menu::Init(){
//     neutral.load("assets/sprites/neutral.png");
//     neutral.pos(0, 0);
//     neutral.size(500, 500);


//     sound.load("assets/audio/Jump.wav");
//     cam = {Vec2::Zero(), 0.0f, 1.0f};
// }

// void Menu::Update(float dt){
//     if (ios::KeyPressed(Keys::E)) {
//         sound.play();
//         core::setScene(std::make_unique<Game>());
//     }
// }

// void Menu::Draw(){
//     mkr::CameraBegin(cam);
//     esys::RenderSprite(neutral.id, neutral);
//     mkr::CameraEnd();
// }

// void Game::Init(){
//     smile.load("assets/sprites/smile.png");
//     smile.size(100, 100);

//     Music music;
//     music.load("assets/audio/t1.ogg");

//     float posX = ((float)mkr::GetWindowWidth()/2) - ((float)smile.tex.width/2);
//     float posY = ((float)mkr::GetWindowHeight()/2) - ((float)smile.tex.height/2);
//     smile.pos(posX, posY);

//     music.play();

//     cam = {Vec2::Zero(), 0.0f, 1.0f};
// }

// void Game::Update(float dt){
//     float speed = 500.0f;
//     if (ios::KeyDown(Keys::W)) smile.position.y -= speed * core::GetDelta();
//     if (ios::KeyDown(Keys::S)) smile.position.y += speed * core::GetDelta();
//     if (ios::KeyDown(Keys::A)) smile.position.x -= speed * core::GetDelta();
//     if (ios::KeyDown(Keys::D)) smile.position.x += speed * core::GetDelta();
// }

// void Game::Draw(){
//     mkr::CameraBegin(cam);
//     esys::RenderSprite(smile.id, smile);
//     mkr::CameraEnd();
// }

int main(){
	bool debug_mode = false;

    if (!debug_mode){
       	script::init_script();
       	script::load_script_file();
    }
    // test code
    else {
    	core::WindowFlag(RESIZABLE);
        core::TargetFPS(60);
        core::MainWindow(800, 600, "Window");
        // core::InitialScene(std::make_unique<Menu>());
    }

    return 0;
}
