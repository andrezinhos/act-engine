#define VAL_IMPL
#include "val.h"

int main() {
    arena_v arena;
    
    val_arenav_init(&arena, 4);
    
    val_arenav_add_position(&arena, 12.0, 13.0, 14.0);
    val_arenav_add_color(&arena, 1.0, 1.0, 1.0);
    val_arenav_add_uv(&arena, 0.0, 0.0);
    
    val_arenav_inc_count(&arena);
    
    val_arenav_add_position(&arena, 22.0, 23.0, 24.0);
    val_arenav_add_color(&arena, 2.0, 2.0, 2.0);
    val_arenav_add_uv(&arena, 1.0, 0.0);
    
    val_arenav_inc_count(&arena);
    
    val_arenav_add_position(&arena, 32.0, 33.0, 34.0);
    val_arenav_add_color(&arena, 3.0, 3.0, 3.0);
    val_arenav_add_uv(&arena, 1.0, 1.0);
    
    val_arenav_inc_count(&arena);
    
    val_arenav_add_position(&arena, 42.0, 43.0, 44.0);
    val_arenav_add_color(&arena, 4.0, 4.0, 4.0);
    val_arenav_add_uv(&arena, 0.0, 1.0);
    
    val_arenav_inc_count(&arena);
    
    val_arenav_check_position(&arena, 4);
    val_arenav_check_color(&arena, 4);
    val_arenav_check_uv(&arena, 4);
    
    val_arenav_free(&arena);
    return 0;
}
