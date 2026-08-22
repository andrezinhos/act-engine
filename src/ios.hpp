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
public:
    static void init_script();
    static void LoadScriptFuncs();
};
