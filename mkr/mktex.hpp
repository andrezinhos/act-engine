#pragma once
#include "mkgl.hpp"

class mktex{
public:
    static Texture DefaultTexture();
    static void UnloadDefaultTexture();

    static Texture LoadTextureSrc(const char* path);
    static void UnloadTexture(const Texture& tex);

    static Image loadImage(const char* path);
    static void unloadImage(Image& image);
    static uint genTex(GLenum type);
    static void setTexParams(GLenum type, GLenum wrap, GLenum format);
    static void setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data);
};
