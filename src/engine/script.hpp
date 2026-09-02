#pragma once
#include "ios.hpp"
#include "core.hpp"
#include "esys.hpp"
#include "scene.hpp"
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

class LuaScene : public Scene{
private:
    sol::table scene_table;
    std::string file;
public:
    LuaScene(const std::string& path) : file(path) {}

    void Init() override{
        scene_table = sol::nil;
        auto result = script::state.do_file(file);
        scene_table = result;
        if (scene_table["Init"].valid()) scene_table["Init"]();
    }

    void Update(float dt) override{
        if (scene_table["Update"].valid()) scene_table["Update"](dt);
    }

    void Draw() override{
        if (scene_table["Draw"].valid()) scene_table["Draw"]();
    }

    void Exit() override{
        if (scene_table["Exit"].valid()) scene_table["Exit"]();
    }
};
