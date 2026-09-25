#include "pwra.h"

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
        free(buffer);
        return NULL;
    }

    if (size) *size = read;
    return buffer;
}

bool LoadSfx(void* data, size_t size, audio_decoder* dec, audio_src* src){
    ma_decoder_config dec_config = ma_decoder_config_init(master.format, master.channels, master.sampleRate);
    ma_result decres = ma_decoder_init_memory(data, size, &dec_config, dec);
    if (decres != MA_SUCCESS){
        check_error("[ERROR] CANNOT DECODE SOUND FILE", decres);
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
        check_error("[ERROR] CANNOT LOAD SOUND FILE:", result);
        return false;
    }

    printf("[INFO] AUDIO FILE LOADED\n");

    return true;
}

bool LoadStream(const char* path, audio_decoder* dec, audio_src* src){
    ma_decoder_config dec_config = ma_decoder_config_init(master.format, master.channels, master.sampleRate);
    ma_result decres = ma_decoder_init_file(path, &dec_config, dec);
    if (decres != MA_SUCCESS){
        check_error("[ERROR] CANNOT DECODE SOUND FILE", decres);
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
        check_error("[ERROR] CANNOT LOAD SOUND FILE:", result);
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
