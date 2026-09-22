#include "pwra.h"
#include "stb_vorbis.c"
#undef L
#undef R
#undef C
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_MP3
#define MA_NO_FLAC
#define MA_NO_RESOURCE_MANAGER
#define MA_NO_WEBAUDIO
#define MA_NO_DSOUND
#define MA_NO_JACK
#define MA_NO_WINMM
#define MA_NO_AUDIO4
#define MA_NO_CUSTOM
#define MA_NO_OPENSL
#define MA_NO_SNDIO
#define MA_NO_OSS

#define MA_ENABLE_WASAPI
#define MA_ENABLE_ALSA
#define MA_ENABLE_COREAUDIO

#define MA_SUPPORT_VORBIS
#include "miniaudio.h"

unsigned char* loadBytes(const char* path, size_t* size){
    FILE* file = fopen(path, "rb");

    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    unsigned char* buffer = malloc(fsize);

    if (!buffer){
        fclose(file);
        return NULL;
    }

    size_t read = fread(buffer, 1, fsize, file);
    fclose(file);
    if (read != fsize){
        audiofree(buffer);
        return NULL;
    }

    if (size) *size = read;
    return buffer;
}

bool LoadSfx(void* data, size_t size, audio_decoder* dec, audio_src* src){
    ma_decoder_config dec_config = ma_decoder_config_init(master.format, master.channels, master.samplesSize);
    ma_result decres = ma_decoder_init_memory(data, size, &dec_config, dec);
    if (decres != MA_SUCCESS){
        printf("[ERROR] CANNOT LOAD SOUND FILE (DECODER)\n");
        return false;
    }

    ma_result result = ma_sound_init_from_data_source(
        &master.engine,
        dec,
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION,
        &master.sound_group,
        src
    );

    if (result != MA_SUCCESS){
        ma_decoder_uninit(dec);
        audiofree(data);
        printf("[ERROR] CANNOT LOAD SOUND FILE\n");
        return false;
    }

    printf("[INFO] AUDIO FILE LOADED\n");

    return true;
}

bool LoadStream(const char* path, audio_decoder* dec, audio_src* src){
    ma_decoder_config dec_config = ma_decoder_config_init(master.format, master.channels, master.samplesSize);
    ma_result decres = ma_decoder_init_file(path, &dec_config, dec);
    if (decres != MA_SUCCESS){
        printf("[ERROR] CANNOT LOAD STREAM FILE (DECODER): %d\n", decres);
        return false;
    }

    ma_result result = ma_sound_init_from_data_source(
        &master.engine,
        dec,
        MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION,
        &master.sound_group,
        src
    );

    if (result != MA_SUCCESS){
        ma_decoder_uninit(dec);
        printf("[ERROR] CANNOT LOAD SOUND FILE: %d\n", result);
        return false;
    }

    printf("[INFO] AUDIO FILE LOADED\n");

    return true;
}

void UnloadSfx(Sfx *sfx){
    ma_sound_uninit(&sfx->source);
    ma_decoder_uninit(&sfx->decoder);
    audiofree(sfx->data);

    printf("[INFO] AUDIO UNLOADED\n");
}

void UnloadStream(Stream *stream){
    ma_sound_uninit(&stream->source);
    ma_decoder_uninit(&stream->decoder);
    printf("[INFO] AUDIO UNLOADED\n");
}
