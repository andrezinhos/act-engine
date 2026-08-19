#pragma once
#include "glfw/glfw3.h"
#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

enum Keys{
    W = GLFW_KEY_W,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
};

class ios{
private:
    static sol::state script;
    static void start_funcs();
    static void start_types();
public:
    static void init_script();
    static void LoadScriptFuncs();
};
