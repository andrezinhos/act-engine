#pragma once
#include "glad.h"
#include "mktypes.h"

namespace mktex{
    Texture DefaultTexture();
    void UnloadDefaultTexture();

    Texture LoadTextureSrc(const char* path);
     void UnloadTexture(Texture* tex);

     Image loadImage(const char* path);
     void unloadImage(Image* image);
     uint genTex(GLenum type);
     void setTexParams(GLenum type, GLenum wrap, GLenum format);
     void setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data);
};
