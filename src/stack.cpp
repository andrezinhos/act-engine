#include "stack.hpp"
#include "mkr.hpp"

std::unordered_map<int, Texture> stack::texmap;
static int count = 0;

int stack::PushSprite(Texture& sprite){
    int id = count++;
    texmap.emplace(id, sprite);
    return id;
}

void stack::UnloadAll(){
    for (auto const& [id, tex] : texmap){
        mkr::UnloadTexture(tex);
    }
    texmap.clear();
    // printf("[INFO] STACK CLEAR\n");
}
