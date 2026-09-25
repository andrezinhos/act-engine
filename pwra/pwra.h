#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pwra_export.h"
#include "miniaudio.h"

typedef ma_sound audio_src;
typedef ma_decoder audio_decoder;

typedef struct {
    audio_src source;
    audio_decoder decoder;
    unsigned char* data;
} Sfx;

typedef struct {
    audio_src source;
    audio_decoder decoder;
} Stream;

typedef struct{
    ma_engine engine;
    ma_device device;
    ma_sound_group sound_group;

    ma_format format;
    int channels;
    int sampleRate;

    ma_backend backend;
    bool custom_opts;
} Master;

#define audiofree(p) do {free(p); p = NULL;} while(0)

#define HIGH 48000
#define LOW 44100

#define MONO 1
#define STEREO 2

#ifdef __cplusplus
extern "C" {
#endif

extern PWRAPI Master master;

PWRAPI void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

PWRAPI void audio_channels(int amount);
PWRAPI void audio_quality(int quality);

PWRAPI void check_error(const char* msg, ma_result res);

PWRAPI bool start_audio();
PWRAPI void end_audio();
PWRAPI void set_master_volume(double vol);

PWRAPI unsigned char* loadBytes(const char* path, size_t* size);
PWRAPI bool LoadSfx(void* data, size_t size, audio_decoder* dec, audio_src* src);
PWRAPI bool LoadStream(const char* path, audio_decoder* dec, audio_src* src);

PWRAPI void UnloadSfx(Sfx* sfx);
PWRAPI void UnloadStream(Stream* stream);

PWRAPI void PlaySfx(Sfx* sfx);
PWRAPI void SetSfxPitch(Sfx* sfx, double value);

PWRAPI void PlayStream(Stream* stream);
PWRAPI void PauseStream(Stream* stream);
PWRAPI void ResumeStream(Stream* stream);
PWRAPI void StopStream(Stream* stream);

#ifdef __cplusplus
}
#endif
