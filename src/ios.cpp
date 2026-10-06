#include "ios.hpp"
#include "mkr.hpp"

int curr[348 + 1];
int prev[348 + 1];

void ios::InputUpdate(){
	for (int key = 0; key <= 348; key++){
		prev[key] = curr[key];

		curr[key] = glfwGetKey(wmain.main, key) == 1;
	}
}

bool ios::KeyDown(Keys key){
	return curr[static_cast<int>(key)];
}

bool ios::KeyPressed(Keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}
