#pragma once
#include "mktypes.h"

namespace mktex{
    Texture DefaultTexture();
    void UnloadDefaultTexture();

    Texture LoadTextureSrc(void* data, int width, int height);
    void UnloadTexture(Texture* tex);

    uint32_t genTex(int format);
    void setTexParams(int type, int wrap, int format);
    void setTexImage2D(int format, int internal, int width, int height, const void* data);
};
