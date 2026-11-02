/*
    VIDEO ARENA LIBRARY

    this is a library I made for making
    more easy to work with memory

    is part of my utility libs, Terrible Libs

    use if you like
*/

#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "tgt.h"

typedef struct arena_v{
    vertex* data;
    size_t cap;
    size_t count;
} arena_v;

typedef struct arena_i{
    uint32_t* data;
    size_t cap;
    size_t count;
} arena_i;

#ifdef VAL_STATIC
    #define VALDEF static inline
#else
    #define VALDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

VALDEF bool val_arenav_init(arena_v* arena, size_t amount);
VALDEF void val_arenav_free(arena_v* arena);

VALDEF void val_arenav_add_position(arena_v* arena, float p1, float p2, float p3);
VALDEF void val_arenav_add_color(arena_v* arena, float p1, float p2, float p3);
VALDEF void val_arenav_add_uv(arena_v* arena, float p1, float p2);

VALDEF void val_arenav_check_position(arena_v* arena, int size);
VALDEF void val_arenav_check_color(arena_v* arena, int size);
VALDEF void val_arenav_check_uv(arena_v* arena, int size);

VALDEF void val_arenav_inc_count(arena_v* arena);
VALDEF void val_arenav_reset(arena_v* arena);

VALDEF size_t val_arenav_size(arena_v* arena);
VALDEF bool val_arenav_limit(arena_v* arena, int value);


VALDEF bool val_arenai_init(arena_i* arena, size_t amount);
VALDEF void val_arenai_free(arena_i* arena);
VALDEF void val_arenai_add_idx(arena_i* arena, int idx);

VALDEF void val_arenai_inc_count(arena_i* arena);
VALDEF void val_arenai_reset(arena_i* arena);

VALDEF size_t val_arenai_size(arena_i* arena);
VALDEF bool val_arenai_limit(arena_i* arena, int value);

#ifdef VAL_IMPL

VALDEF bool val_arenav_init(arena_v* arena, size_t amount){
    arena->data = (vertex*)malloc(sizeof(vertex) * amount);

    if (arena->data == NULL){
        printf("error: failed to init vertex arena\n");
        return false;
    }

    arena->cap = amount;
    arena->count = 0;

    return true;
}

VALDEF void val_arenav_free(arena_v* arena){
    free(arena->data);
    arena->data = NULL;
    arena->cap = 0;
    arena->count = 0;
}

VALDEF void val_arenav_add_position(arena_v* arena, float p1, float p2, float p3){
    float value[3] = {p1, p2, p3};
    memcpy(
        arena->data[arena->count].position,
        value, sizeof(value)
    );
}

VALDEF void val_arenav_add_color(arena_v* arena, float p1, float p2, float p3){
    float value[3] = {p1, p2, p3};
    memcpy(
        arena->data[arena->count].color,
        value, sizeof(value)
    );
}

VALDEF void val_arenav_add_uv(arena_v* arena, float p1, float p2){
    float value[2] = {p1, p2};
    memcpy(
        arena->data[arena->count].uv,
        value, sizeof(value)
    );
}

VALDEF void val_arenav_check_position(arena_v* arena, int size){
    for (int i = 0; i < size; i++){
        printf("arena position value : index %d : %.1f | %.1f | %.1f\n", 
            i,
            arena->data[i].position[0],
            arena->data[i].position[1],
            arena->data[i].position[2]
        );
    }
}

VALDEF void val_arenav_check_color(arena_v* arena, int size){
    for (int i = 0; i < size; i++){
        printf("arena color value : index %d : %.1f | %.1f | %.1f\n", 
            i,
            arena->data[i].color[0],
            arena->data[i].color[1],
            arena->data[i].color[2]
        );
    }
}

VALDEF void val_arenav_check_uv(arena_v* arena, int size){
    for (int i = 0; i < size; i++){
        printf("arena uv value : index %d : %.1f | %.1f\n", 
            i,
            arena->data[i].uv[0],
            arena->data[i].uv[1]
        );
    }
}

VALDEF void val_arenav_inc_count(arena_v* arena){
    arena->count += 1;
}

VALDEF void val_arenav_reset(arena_v* arena){
    if (arena->count > 0) {
        arena->count = 0;
    }
}

VALDEF size_t val_arenav_size(arena_v* arena){
    return arena->count;
}

VALDEF bool val_arenav_limit(arena_v* arena, int value){
    bool result;
    
    if (arena->cap >= value){
        result = true;
    }
    else {
        result = false;
    }
    
    return result;
}


VALDEF bool val_arenai_init(arena_i* arena, size_t amount){
    arena->data = (uint32_t*)malloc(sizeof(uint32_t) * amount);
    
    if (arena->data == NULL){
        printf("error: failed to init indices arena\n");
        return false;
    }
    
    arena->cap = amount;
    arena->count = 0;
    
    return true;
}

VALDEF void val_arenai_free(arena_i* arena){
    free(arena->data);
    arena->data = NULL;
    arena->cap = 0;
    arena->count = 0;
}

VALDEF void val_arenai_add_idx(arena_i* arena, int idx){
    arena->data[arena->count] = idx;
}

VALDEF void val_arenai_inc_count(arena_i* arena){
    arena->count += 1;
}

VALDEF void val_arenai_reset(arena_i* arena){
    if (arena->count > 0) {
        arena->count = 0;
    }
}

VALDEF size_t val_arenai_size(arena_i* arena){
    return arena->count;
}

VALDEF bool val_arenai_limit(arena_i* arena, int value){
    bool result;
    
    if (arena->cap >= value){
        result = true;
    }
    else {
        result = false;
    }
    
    return result;
}

#endif // VAL_IMPL

#ifdef __cplusplus
}
#endif
