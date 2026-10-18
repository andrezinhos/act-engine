#pragma once
#include <cstdint>

struct Entity{
    uint32_t id;
    bool active;
};

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};

namespace lecs{

class ents{
public:

};

};
