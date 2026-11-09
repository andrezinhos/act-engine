#pragma once
#include "core.hpp"
#include <string>

class lscene : public engine::scene {
private:
    std::string file;
public:
    lscene(std::string_view path) : file(path) {}
    void init() override;
    void update(float dt) override;
    void draw() override;
    void exit() override;
};
