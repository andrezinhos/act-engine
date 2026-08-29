#include "ios.hpp"
#include "stack.hpp"


int ios::curr[348 + 1];
int ios::prev[348 + 1];

void ios::InputUpdate(){
	for (int key = 0; key <= 348; key++){
		prev[key] = curr[key];

		curr[key] = glfwGetKey(mkr::wmain.main, key) == GLFW_PRESS;
	}
}

bool ios::KeyDown(Keys key){
	return curr[static_cast<int>(key)];
}

bool ios::KeyPressed(Keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}

Texture ios::LoadTexture(const std::string &path){
    Texture tex = mkr::LoadTextureSrc(path.c_str());
    // stack::PushSprite(tex);
    return tex;
}
