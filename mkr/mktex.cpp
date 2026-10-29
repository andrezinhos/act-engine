#include "mktex.hpp"
#include "fgl.h"
#include "mkr.hpp"

uint32_t mktex::genTex(int type){
    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(type, tex);
    return tex;
}

void mktex::setTexParams(int type, int wrap, int format){
    glTexParameteri(type, wrap, format);
}

void mktex::setTexImage2D(int format, int internal, int width, int height, const void* data){
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture mktex::DefaultTexture(){
    Texture tex;

    uint32_t color = 0xFFFFFFFF;
    fgl_create_texture(&tex.id, GL_TEXTURE_2D, 1, 1, &color);
    
    fgl_texture_repeat(tex.id);
    fgl_texture_filter(tex.id, GL_NEAREST);

    return tex;
}

void mktex::UnloadDefaultTexture(){
    if (dstate->dtex.id != 0) {
        glDeleteTextures(1, &dstate->dtex.id);
    }
}

Texture mktex::LoadTextureSrc(void* data, int width, int height){
    Texture tex;
    tex.width = width;
    tex.height = height;

    fgl_create_texture(&tex.id, GL_TEXTURE_2D, width, height, data);
    fgl_texture_repeat(tex.id);
    fgl_texture_filter(tex.id, GL_NEAREST);

    return tex;
}

void mktex::UnloadTexture(Texture* tex){
    if (tex->id != 0) glDeleteTextures(1, &tex->id);
    printf("[INFO] TEXTURE UNLOADED\n");
}

void mkr::RenderRectangle(Vec2 position, Vec2 size, color color){
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

void mkr::RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, color color){
    limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

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

void mkr::RenderTexture(Texture *tex, Vec2 position, Vec2 size, color color){
    limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

    sendVertex(position, size, color, {0.0f, 1.0f});
    sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != tex){
        dstate->dbatch.calls.push_back({indexStart, 6, tex});
    }
    else dstate->dbatch.calls.back().count += 6;
}
