#include "mkr.hpp"
#include "mktex.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_GIF
#define STBI_NO_PNM
#define STBI_NO_BMP
#define STBI_NO_PIC
#define STBI_NO_PSD
#define STBI_NO_HDR
#include "stb_image.h"

Image mktex::loadImage(const char* path){
    Image image = {};

    size_t imgsize = 0;
    byte* imgbuf = mkgl::loadBytes(path, &imgsize);

    image.data = stbi_load_from_memory(
        imgbuf,
        imgsize,
        &image.width,
        &image.height,
        &image.channels,
        4
    );

    freeptr(imgbuf);
    return image;
}

void mktex::unloadImage(Image& image){
    stbi_image_free(image.data);
}

uint mktex::genTex(GLenum type){
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(type, tex);
    return tex;
}

void mktex::setTexParams(GLenum type, GLenum wrap, GLenum format){
    glTexParameteri(type, wrap, format);
}

void mktex::setTexImage2D(GLenum format, GLenum internal, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture mktex::DefaultTexture(){
    Texture tex;
    tex.id = genTex(GL_TEXTURE_2D);

    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint color = 0xFFFFFFFF;
    setTexImage2D(GL_RGBA, GL_RGBA, 1, 1, &color);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void mktex::UnloadDefaultTexture(){
    if (dstate->dtex.id != 0) glDeleteTextures(1, &dstate->dtex.id);
}

Texture mktex::LoadTextureSrc(const char* path){
    Image image = mktex::loadImage(path);
    Texture tex;
    tex.id = mktex::genTex(GL_TEXTURE_2D);
    tex.width = image.width;
    tex.height = image.height;

    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    setTexImage2D(GL_RGBA, GL_RGBA8, image.width, image.height, image.data);
    unloadImage(image);

    return tex;
}

void mktex::UnloadTexture(const Texture& tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("[INFO] TEXTURE UNLOADED\n");
}

void mkr::RenderRectangle(Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

    sendVertex(position, size, color, {0.0f, 1.0f});
    sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != &dstate->dtex){
        dstate->dbatch.calls.push_back({indexStart, 6, &dstate->dtex});
    }
    else dstate->dbatch.calls.back().count += 6;
}

void mkr::RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint base = dstate->dbatch.vertices.size();
    uint indexStart = dstate->dbatch.indices.size();

    float u0 = rectangle.x / (float)tex->width;
    float v0 = rectangle.y / (float)tex->height;

    float u1 = (rectangle.x + (float)rectangle.width) / (float)tex->width;
    float v1 = (rectangle.y + (float)rectangle.height) / (float)tex->height;

    sendVertex(position, size, color, u0, v0, u1, v1);
    sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != tex){
        dstate->dbatch.calls.push_back({indexStart, 6, tex});
    }
    else dstate->dbatch.calls.back().count += 6;
}

void mkr::RenderTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint base = dstate->dbatch.vertices.size();
    uint indexStart = dstate->dbatch.indices.size();

    sendVertex(position, size, color, {0.0f, 1.0f});
    sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != tex){
        dstate->dbatch.calls.push_back({indexStart, 6, tex});
    }
    else dstate->dbatch.calls.back().count += 6;
}
