#include "mktex.hpp"
#include "mkgl.hpp"
#include "mkr.hpp"

uint mktex::genTex(int type){
    uint tex;
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

Texture mktex::LoadTextureSrc(void* data, int width, int height){
    Texture tex;
    tex.id = mktex::genTex(GL_TEXTURE_2D);
    tex.width = width;
    tex.height = height;

    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    setTexImage2D(GL_RGBA, GL_RGBA8, width, height, data);
    return tex;
}

void mktex::UnloadTexture(Texture* tex){
    if (tex->id != 0) glDeleteTextures(1, &tex->id);
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
