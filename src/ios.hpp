#pragma once
#include "core.hpp"

enum class Keys {
    W = 87,
    A = 65,
    S = 83,
    D = 68,
};

class ios{
private:
    static void start_funcs();
    static void start_types();

    static int curr[GLFW_KEY_LAST + 1];
    static int prev[GLFW_KEY_LAST + 1];
public:
    static void init_script();
    static void LoadScriptFuncs();


	static void InputUpdate();
    static bool KeyDown(Keys key);
    static bool KeyPressed(Keys key);
};
