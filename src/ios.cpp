#include "ios.hpp"
#include "mkr.hpp"
#include <array>

constexpr int MAXKEY = 350;

std::array<int, MAXKEY> prev = {};
std::array<int, MAXKEY> curr = {};

void ios::InputUpdate(){
    prev = curr;

	for (int key = 0; key < MAXKEY; key++)
		curr.at(key) = glfwGetKey(wmain.main, key) == 1;
}

bool ios::KeyDown(Keys key){
	return curr[static_cast<int>(key)];
}

bool ios::KeyPressed(Keys key){
	return curr[static_cast<int>(key)] && !prev[static_cast<int>(key)];
}
