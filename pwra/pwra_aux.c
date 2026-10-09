#include "pwra.h"

bool pwra_load_sfx(void* data, size_t size, audio_decoder* dec, audio_src* src){
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

bool pwra_load_stream(const char* path, audio_decoder* dec, audio_src* src){
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

    printf("[INFO] AUDIO FILE LOADED: %s\n", path);
    return true;
}

void pwra_unload_sfx(Sfx *sfx){
    ma_sound_uninit(&sfx->source);
    ma_decoder_uninit(&sfx->decoder);
    free(sfx->data);
    sfx->data = NULL;
    printf("[INFO] AUDIO UNLOADED\n");
}

void pwra_unload_stream(Stream *stream){
    ma_sound_uninit(&stream->source);
    ma_decoder_uninit(&stream->decoder);
    printf("[INFO] AUDIO UNLOADED\n");
}
