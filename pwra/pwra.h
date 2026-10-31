#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pwra_export.h"
#include "miniaudio.h"

typedef ma_sound audio_src;
typedef ma_decoder audio_decoder;

typedef struct Sfx {
    audio_src source;
    audio_decoder decoder;
    uint8_t* data;
} Sfx;

typedef struct Stream {
    audio_src source;
    audio_decoder decoder;
} Stream;

typedef struct{
    ma_engine engine;
    ma_sound_group sound_group;

    int channels;
    int sampleRate;

    int custom_opts;
} Master;

#define PWRA_SUCCESS 0;

#ifdef __cplusplus
extern "C" {
#endif

extern PWRAPI Master master;

PWRAPI void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void check_error(const char* msg, ma_result res);

PWRAPI int pwra_start_audio();
PWRAPI void pwra_end_audio();
PWRAPI void pwra_set_master_volume(double vol);

PWRAPI int pwra_load_sfx(const char* path, Sfx* sfx);
PWRAPI int pwra_load_stream(const char* path, Stream* stream);

PWRAPI void pwra_unload_sfx(Sfx* sfx);
PWRAPI void pwra_unload_stream(Stream* stream);

PWRAPI void pwra_play_sfx(Sfx* sfx);
PWRAPI void pwra_set_pitch(Sfx* sfx, double value);

PWRAPI void pwra_play_stream(Stream* stream);
PWRAPI void pwra_pause_stream(Stream* stream);
PWRAPI void pwra_resume_stream(Stream* stream);
PWRAPI void pwra_stop_stream(Stream* stream);

#ifdef __cplusplus
}
#endif
