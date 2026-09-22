#include "pwra.h"

Master master;

void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    ma_engine_read_pcm_frames(
        &master.engine,
        pOutput,
        frameCount,
        NULL
    );

    (void)pInput;
}

bool start_audio(){
    ma_engine_config e_config = ma_engine_config_init();

    master.channels = 2;
    master.samplesSize = 48000;
    master.format = ma_format_f32;

    e_config.channels = master.channels;
    e_config.sampleRate = master.samplesSize;
    e_config.noAutoStart = MA_TRUE;

    ma_result einit = ma_engine_init(&e_config, &master.engine);
    if (einit != MA_SUCCESS){
        printf("[ERROR] ma_engine_init failed with code: %d\n", einit);
        return false;
    }

    ma_result sginit = ma_sound_group_init(&master.engine, 0, NULL, &master.sound_group);
    if (sginit != MA_SUCCESS){
        printf("[ERROR] COULDN'T INITIALIZE AUDIO (ma_result: %d)\n", sginit);
        return false;
    }

    ma_result estart = ma_engine_start(&master.engine);
    if (estart != MA_SUCCESS){
        printf("[ERROR] COULDN'T INITIALIZE AUDIO (ma_result: %d)\n", estart);
        ma_engine_uninit(&master.engine);
        return false;
    }

    printf("[INFO] AUDIO INITIALIZED\n");

    return true;
}

void end_audio(){
    ma_engine_stop(&master.engine);
    ma_sound_group_uninit(&master.sound_group);
    ma_engine_uninit(&master.engine);

    printf("[INFO] AUDIO UNINITIALIZED\n");
}

void set_master_volume(double vol){
    ma_sound_group_set_volume(&master.sound_group, vol);
}

void PlaySfx(Sfx* sfx){
    ma_sound_seek_to_pcm_frame(&sfx->source, 0);
    ma_sound_start(&sfx->source);
}

void SetSfxPitch(Sfx *sfx, double value){
    ma_sound_set_pitch(&sfx->source, value);
}

void PlayStream(Stream* stream){
    ma_sound_start(&stream->source);
}

void PauseStream(Stream* stream){
    ma_sound_stop(&stream->source);
}

void ResumeStream(Stream* stream){
    ma_sound_start(&stream->source);
}

void StopStream(Stream* stream){
    ma_sound_stop(&stream->source);
    ma_sound_seek_to_pcm_frame(&stream->source, 0);
}
