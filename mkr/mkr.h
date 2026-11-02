#pragma once
#include <stdbool.h>
#include "tgt.h"

extern shader dshader;

#ifdef __cplusplus
extern "C" {
#endif

extern bool mk_init();
extern void mk_shutdown();

extern bool mk_load_default_shader();
extern void mk_unload_default_shader();

#ifdef __cplusplus
}
#endif
