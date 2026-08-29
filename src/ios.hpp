#pragma once
#include "core.hpp"
#include <string>

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

    static int curr[348 + 1];
    static int prev[348 + 1];
public:
    static void init_script();
    static void LoadScriptFuncs();

	static void InputUpdate();
    static bool KeyDown(Keys key);
    static bool KeyPressed(Keys key);

    static Texture LoadTexture(const std::string& path);
};
