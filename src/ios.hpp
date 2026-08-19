#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

class ios{
private:
    static sol::state script;
    static void start_funcs();
    static void start_types();
public:
    static void init_script();
    static void LoadScriptFuncs();
};
