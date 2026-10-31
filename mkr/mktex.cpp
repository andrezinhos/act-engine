#include "mktex.hpp"
#include "fgl.h"
#include "mkr.hpp"

int mktex::create_tex(uint32_t* id, int type, int width, int height, void* data){
    return fgl_create_texture(id, type, width, height, data);
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

Texture mktex::load_tex_src(void* data, int width, int height){
    Texture tex;
    tex.width = width;
    tex.height = height;

    fgl_create_texture(&tex.id, GL_TEXTURE_2D, width, height, data);
    fgl_texture_repeat(tex.id);
    fgl_texture_filter(tex.id, GL_NEAREST);

    return tex;
}

void mktex::unload_tex(Texture* tex){
    if (tex->id != 0) glDeleteTextures(1, &tex->id);
    printf("[INFO] TEXTURE UNLOADED\n");
}

void mkr::RenderRectangle(Vec2 position, Vec2 size, color color){
    limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

    send_vertex(position, size, color, 0.0f, 0.0f, 1.0f, 1.0f);
    sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != &dstate->dtex){
        dstate->dbatch.calls.push_back({indexStart, 6, &dstate->dtex});
    }
    else dstate->dbatch.calls.back().count += 6;
}

void mktex::render_tex_rect(Texture* tex, Rectangle rectangle, Vec2 position, Vec2 size, color color){
    mkr::limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

    float u0 = rectangle.x / (float)tex->width;
    float v0 = rectangle.y / (float)tex->height;

    float u1 = (rectangle.x + (float)rectangle.width) / (float)tex->width;
    float v1 = (rectangle.y + (float)rectangle.height) / (float)tex->height;

    mkr::send_vertex(position, size, color, u0, v0, u1, v1);
    mkr::sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != tex){
        dstate->dbatch.calls.push_back({indexStart, 6, tex});
    }
    else dstate->dbatch.calls.back().count += 6;
}

void mktex::render_tex(Texture *tex, Vec2 position, Vec2 size, color color){
    mkr::limitFlush();
    uint32_t base = dstate->dbatch.vertices.size();
    uint32_t indexStart = dstate->dbatch.indices.size();

    mkr::send_vertex(position, size, color, 0.0f, 0.0f, 1.0f, 1.0f);
    mkr::sendIndices(base);

    if (dstate->dbatch.calls.empty() || dstate->dbatch.calls.back().texref != tex){
        dstate->dbatch.calls.push_back({indexStart, 6, tex});
    }
    else dstate->dbatch.calls.back().count += 6;
}

int mktex::tex_valid(uint32_t *tex){
    if (tex == 0){
        printf("TEXTURE INVALID: id %d\n", *tex);
        return 1;
    }
    else {
        printf("TEXTURE VALID: id %d\n", *tex);
        return 0;
    }
}
