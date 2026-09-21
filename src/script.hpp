#pragma once
#include "scene.hpp"
#include <string>

namespace script{
    void start_funcs();
    void start_types();
    void load_conteiners();
    void init_script();
    void load_script_file();
};

class LuaScene : public Scene{
private:
    std::string file;
public:
    LuaScene(const std::string& path) : file(path) {}
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Exit() override;
};
