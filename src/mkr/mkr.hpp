#pragma once
#include "mkgl.hpp"

class mkr{
public:
    mkr();
    ~mkr();
    static Shader DefaultShader();
    static Mesh DefaultQuad();
    static void DefaultBatch();

    static void UnloadDefaultShader();
    static void UnloadDefaultQuad();
    static void UnloadDefaultBatch();
};
