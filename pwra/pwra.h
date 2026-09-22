#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

static const int HIGH = 48000;
static const int LOW = 44100;

static const int MONO = 1;
static const int STEREO = 2;

#ifdef __cplusplus
extern "C" {
#endif

extern Master master;

void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void audio_channels(int amount);
void audio_quality(int quality);

void check_error(const char* msg, ma_result res);

bool start_audio();
void end_audio();
void set_master_volume(double vol);

unsigned char* loadBytes(const char* path, size_t* size);
bool LoadSfx(void* data, size_t size, audio_decoder* dec, audio_src* src);
bool LoadStream(const char* path, audio_decoder* dec, audio_src* src);

void UnloadSfx(Sfx* sfx);
void UnloadStream(Stream* stream);

void PlaySfx(Sfx* sfx);
void SetSfxPitch(Sfx* sfx, double value);

void PlayStream(Stream* stream);
void PauseStream(Stream* stream);
void ResumeStream(Stream* stream);
void StopStream(Stream* stream);

#ifdef __cplusplus
}
#endif
