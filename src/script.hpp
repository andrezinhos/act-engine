#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "sol.hpp"

namespace code{
    inline sol::state script;

    void start_types();
    void init_script();
    void start_funcs();
    void LoadScriptFuncs();
};
