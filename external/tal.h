#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tgt.h"

typedef struct arena_v {
    vertex* data;
    size_t cap;
    size_t count;
} arena_v;

#ifdef TAL_STATIC
    #define TALDEF static
#else
    #define TALDEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

TALDEF int arenav_init(arena_v* arena, size_t size);
TALDEF void arenav_free(arena_v* arena);

TALDEF int arenav_valid(arena_v* arena);
TALDEF void arenav_add_position(arena_v* arena, int idx, float p1, float p2, float p3);
TALDEF void arenav_add_color(arena_v* arena, int idx, float p1, float p2, float p3);
TALDEF void arenav_add_uv(arena_v* arena, int idx, float p1, float p2);

TALDEF void arenav_check_position(arena_v* arena, int index);

#ifdef TAL_IMPL

TALDEF int arenav_init(arena_v* arena, size_t size){
    arena->data = malloc(sizeof(arena_v) * size);

    if (arena->data == NULL) {
        printf("error: failed to init arena");
        return 1;
    }

    arena->cap = size;
    arena->count = 0;

    return 0;
}

TALDEF void arenav_free(arena_v* arena){
    free(arena->data);
    arena->data = NULL;
    arena->cap = 0;
    arena->count = 0;
}

TALDEF int arenav_valid(arena_v* arena){
    int result;

    if (arena->data){
        printf("ARENA NULL");
        result = 0;
    }
    else {
        printf("ARENA OK");
        result = 1;
    }

    return result;
}

TALDEF void arenav_add_position(arena_v* arena, int idx, float p1, float p2, float p3){
    float value[3] = {p1, p2, p3};
    memcpy(arena->data[idx].position, value, sizeof(value));
}

TALDEF void arenav_add_color(arena_v* arena, int idx, float p1, float p2, float p3){
    float value[3] = {p1, p2, p3};
    memcpy(arena->data[idx].color, value, sizeof(value));
}

TALDEF void arenav_add_uv(arena_v* arena, int idx, float p1, float p2){
    float value[2] = {p1, p2};
    memcpy(arena->data[idx].uv, value, sizeof(value));
}

TALDEF void arenav_check_position(arena_v* arena, int index){
    for (int i = 0; i < index; i++){
        printf("arena position: %.1f | %.1f | %.1f\n"
               arena->data[i].position[0],
               arena->data[i].position[1],
               arena->data[i].position[2],
        );
    }
}

#endif

#ifdef __cplusplus
}
#endif
