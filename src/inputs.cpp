#include "ios.hpp"

int ios::curr[GLFW_KEY_LAST + 1];
int ios::prev[GLFW_KEY_LAST + 1];

void ios::InputUpdate(){
	for (int key = 0; key <= GLFW_KEY_LAST; key++){
		prev[key] = curr[key];

		curr[key] = glfwGetKey(core::state.win, key) == GLFW_PRESS;
	}
}

bool ios::KeyDown(Keys key){
	return curr[static_cast<int>(key)];
}

bool ios::KeyPressed(Keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}
