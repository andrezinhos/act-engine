#include "mkr.hpp"

Texture mkr::DefaultTexture(){
    Texture tex;
    tex.id = mkgl::genTex(GL_TEXTURE_2D);

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint color = 0xFFFFFFFF;
    mkgl::setTexImage2D(GL_RGBA, GL_RGBA, 1, 1, &color);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void mkr::UnloadDefaultTexture(){
    if (state.dtex.id != 0) glDeleteTextures(1, &state.dtex.id);
}

Texture mkr::LoadTextureSrc(const char* path){
    Image image = mkgl::loadImage(path);
    Texture tex;
    tex.id = mkgl::genTex(GL_TEXTURE_2D);
    tex.width = image.width;
    tex.height = image.height;

    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    mkgl::setTexParams(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mkgl::setTexImage2D(GL_RGBA, GL_RGBA, image.width, image.height, image.data);
    mkgl::unloadImage(image);

    return tex;
}

void mkr::UnloadTexture(const Texture& tex){
    if (tex.id != 0) glDeleteTextures(1, &tex.id);
    printf("[INFO] TEXTURE UNLOADED\n");
}

void mkr::RenderRectangle(Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint32_t base = state.dbatch.vertices.size();
    uint32_t indexStart = state.dbatch.indices.size();

    sendVertex(position, size, color, {0.0f, 1.0f});
    sendIndices(base);
    
    if (state.dbatch.calls.empty() || state.dbatch.calls.back().texref != &state.dtex){
        state.dbatch.calls.push_back({indexStart, 6, &state.dtex});
    }
    else state.dbatch.calls.back().count += 6;
}

void mkr::RenderTextureRec(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint base = state.dbatch.vertices.size();
    uint indexStart = state.dbatch.indices.size();

    float u0 = rectangle.x / (float)tex->width;
    float v0 = rectangle.y / (float)tex->height;

    float u1 = (rectangle.x + (float)rectangle.width) / (float)tex->width;
    float v1 = (rectangle.y + (float)rectangle.height) / (float)tex->height;

    sendVertex(position, size, color, u0, v0, u1, v1);
    sendIndices(base);

    if (state.dbatch.calls.empty() || state.dbatch.calls.back().texref != tex){
        state.dbatch.calls.push_back({indexStart, 6, tex});
    }
    else state.dbatch.calls.back().count += 6;
}

void mkr::RenderTexture(Texture *tex, Vec2 position, Vec2 size, Color color){
    limitFlush();
    uint base = state.dbatch.vertices.size();
    uint indexStart = state.dbatch.indices.size();

    sendVertex(position, size, color, {0.0f, 1.0f});
    sendIndices(base);

    if (state.dbatch.calls.empty() || state.dbatch.calls.back().texref != tex){
        state.dbatch.calls.push_back({indexStart, 6, tex});
    }
    else state.dbatch.calls.back().count += 6;
}
