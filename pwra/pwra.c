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

void audio_channels(int amount){
    master.custom_opts = true;
    master.channels = amount;
}

void audio_quality(int quality){
    master.custom_opts = true;
    master.sampleRate = quality;
}

bool start_audio(){
    ma_engine_config e_config = ma_engine_config_init();

    master.format = ma_format_f32;

    if (!master.custom_opts){
        master.channels = 1;
        master.sampleRate = 44100;
    }

    e_config.channels = master.channels;
    e_config.sampleRate = master.sampleRate;
    e_config.noAutoStart = MA_TRUE;

    ma_result einit = ma_engine_init(&e_config, &master.engine);
    if (einit != MA_SUCCESS){
        printf("[ERROR] COULDN'T INITIALIZE AUDIO: %d\n", einit);
        return false;
    }

    ma_result sginit = ma_sound_group_init(&master.engine, 0, NULL, &master.sound_group);
    if (sginit != MA_SUCCESS){
        printf("[ERROR] COULDN'T INITIALIZE AUDIO: %d\n", sginit);
        return false;
    }

    ma_result estart = ma_engine_start(&master.engine);
    if (estart != MA_SUCCESS){
        printf("[ERROR] COULDN'T INITIALIZE AUDIO: %d\n", estart);
        ma_engine_uninit(&master.engine);
        return false;
    }

    printf("[INFO] AUDIO INITIALIZED\n");

#ifdef _WIN32
    printf("[INFO] AUDIO BACKEND (WINDOWS): %s\n", ma_get_backend_name(master.backend));
#elif __linux__
    printf("[INFO] AUDIO BACKEND (LINUX): %s\n", ma_get_backend_name(master.backend));
#elif __APPLE__
    printf("[INFO] AUDIO BACKEND (MAC): %s\n", ma_get_backend_name(master.backend));
#endif
    return true;
}

void end_audio(){
    ma_engine_stop(&master.engine);
    ma_sound_group_stop(&master.sound_group);
    ma_sound_group_uninit(&master.sound_group);
    ma_engine_uninit(&master.engine);

    printf("[INFO] AUDIO UNINITIALIZED\n");
}


void check_error(const char *msg, ma_result res){
    printf("%s: ", msg);

    if (res == -2) printf("INVALID ARGS");
    else if (res == -3) printf("OUT OF MEMORY");
    else if (res == -4) printf("OUT OF RANGE");
    else if (res == -5) printf("ACCESS DENIED");
    else if (res == -6) printf("DOES NOT EXIST");
    else if (res == -7) printf("NO SPACE");
    else if (res == -8) printf("BAD MESSAGE");
    else if (res == -9) printf("BUSY");
    else if (res == -10) printf("IO ERROR");
    else if (res == -100) printf("NOT SUPPORTED FILE");

    printf("\n");
}

void set_master_volume(double vol){
    ma_sound_group_set_volume(&master.sound_group, vol);
}

void PlaySfx(Sfx* sfx){
    ma_sound_start(&sfx->source);
    ma_sound_seek_to_pcm_frame(&sfx->source, 0);
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
