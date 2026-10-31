#include "pwra.h"
#include <stddef.h>
#include <stdint.h>

uint8_t* pwra_read_file(const char* path, size_t* size){
    FILE* file = fopen(path, "rb");

    if (!file){
        printf("error: audio file do not exist\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    uint8_t* buffer = (uint8_t*)malloc(fsize);

    if (!buffer){
        printf("error: couldn't create audio file buffer\n");
        return NULL;
    }

    size_t read = fread(buffer, 1, fsize, file);
    fclose(file);

    if (read != fsize){
        printf("error: couldn't read file bytes\n");
        free(buffer);
        return NULL;
    }

    *size = fsize;
    return buffer;
}

int pwra_load_sfx(const char* path, Sfx* sfx){
    size_t size = 0;
    sfx->data = pwra_read_file(path, &size);

    ma_decoder_config dec_config = ma_decoder_config_init_default();
    ma_result decres = ma_decoder_init_memory(sfx->data, size, &dec_config, &sfx->decoder);

    if (decres != MA_SUCCESS){
        check_error("[ERROR] CANNOT DECODE SOUND FILE", decres);
        return 1;
    }

    ma_result result = ma_sound_init_from_data_source(
        &master.engine,
        &sfx->decoder,
        MA_SOUND_FLAG_NO_SPATIALIZATION,
        &master.sound_group,
        &sfx->source
    );

    if (result != MA_SUCCESS){
        ma_decoder_uninit(&sfx->decoder);
        free(sfx->data);
        check_error("[ERROR] CANNOT LOAD SOUND FILE:", result);
        return 1;
    }

    printf("[INFO] AUDIO FILE LOADED: %s\n", path);

    return 0;
}

int pwra_load_stream(const char* path, Stream* stream){

    ma_decoder_config dec_config = ma_decoder_config_init_default();
    ma_result decres = ma_decoder_init_file(path, &dec_config, &stream->decoder);
    if (decres != MA_SUCCESS){
        check_error("[ERROR] CANNOT DECODE SOUND FILE", decres);
        return 1;
    }

    ma_result result = ma_sound_init_from_data_source(
        &master.engine,
        &stream->decoder,
        MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH,
        &master.sound_group,
        &stream->source
    );

    if (result != MA_SUCCESS){
        ma_decoder_uninit(&stream->decoder);
        check_error("[ERROR] CANNOT LOAD SOUND FILE:", result);
        return 1;
    }

    printf("[INFO] AUDIO FILE LOADED: %s\n", path);
    return 0;
}

void pwra_unload_sfx(Sfx *sfx){
    ma_sound_uninit(&sfx->source);
    ma_decoder_uninit(&sfx->decoder);
    free(sfx->data);
    printf("[INFO] AUDIO UNLOADED\n");
}

void pwra_unload_stream(Stream *stream){
    ma_sound_uninit(&stream->source);
    ma_decoder_uninit(&stream->decoder);
    printf("[INFO] AUDIO UNLOADED\n");
}
