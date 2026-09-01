#pragma once
#include "ios.hpp"
#include "core.hpp"
#include "esys.hpp"
#include "sol.hpp"
#include <string>

namespace script{
    inline sol::state state;
    void start_funcs();
    void start_types();
    void load_conteiners();
    void init_script();
    void load_script_file();
};
