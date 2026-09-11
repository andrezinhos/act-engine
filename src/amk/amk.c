#include "amk.h"
#include "miniaudio.h"

Master master = {};

void callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    Master* master = (Master*)(pDevice->pUserData);

    ma_node_graph_read_pcm_frames(&master->ngraph, pOutput, frameCount, NULL);
}

bool initAudioDevice(){
    master.format = ma_format_f32;
    master.channels = 2;
    master.sampleRate = 48000;
    master.master_vol = 1.0f;

    ma_node_graph_config graph_config = ma_node_graph_config_init(master.channels);

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = master.format;
    config.playback.channels = master.channels;
    config.sampleRate = master.sampleRate;
    config.dataCallback = callback;
    config.pUserData = &master;

    if (ma_device_init(NULL, &config, &master.device) != MA_SUCCESS){
        printf("[INFO] ERROR TO INTIALIZE AUDIO (DEVICE)\n");
        return false;
    }

    ma_result result = ma_device_start(&master.device);
    if (result != MA_SUCCESS){
        printf("[INFO] ERROR TO INTIALIZE AUDIO (START DEVICE)\n");
        return false;
    }

    ma_device_set_master_volume(&master.device, master.master_vol);

    if (ma_node_graph_init(&graph_config, NULL, &master.ngraph) != MA_SUCCESS) {
        printf("[INFO] ERROR TO INTIALIZE AUDIO (NODE GRAPH)\n");
        return false;
    }

    printf("[INFO] AUDIO DEVICE STARTED\n");
    return true;
}

void endAudioDevice(){
    ma_device_stop(&master.device);
    ma_device_uninit(&master.device);
    ma_node_graph_uninit(&master.ngraph, NULL);
    printf("[INFO] AUDIO DEVICE CLOSED\n");
}

void LoadSoundAudioFile(unsigned char* data, size_t size, Decoder* dec, NodeSource* node){
    if (!initDecoder(data, size, dec)) {
        free(data);
        return;
    }
    if (!getSourceData(node, dec)){
        unloadDecoder(dec);
        free(data);
        return;
    }

    if (!sendOutput(node)){
        unloadSourceData(node);
        unloadDecoder(dec);
        free(data);
        return;
    }

    printf("[INFO] AUDIO FILE LOADED\n");
}

void LoadMusicAudioFile(const char* data, Decoder* dec, NodeSource* node){
    if (!initDecoderPath(data, dec)) {
        return;
    }
    if (!getSourceData(node, dec)){
        unloadDecoder(dec);
        return;
    }

    if (!sendOutput(node)){
        unloadSourceData(node);
        unloadDecoder(dec);
        return;
    }

    printf("[INFO] AUDIO FILE LOADED\n");
}

void PlayAudioFile(Decoder* dec, NodeSource *node){
    ma_data_source_seek_to_pcm_frame(dec, 0);
    ma_node_set_state(node, ma_node_state_started);
}

void PauseAudioFile(NodeSource *node){
    ma_node_set_state(node, ma_node_state_stopped);
}

void ResumeAudioFile(NodeSource* node){
    ma_node_set_state(node, ma_node_state_started);
}

void StopAudioFile(Decoder* dec, NodeSource *node){
    ma_node_set_state(node, ma_node_state_stopped);
    ma_data_source_seek_to_pcm_frame(dec, 0);
}

void UnloadAudioFile(Decoder *dec, NodeSource* node){
    unloadSourceData(node);
    unloadDecoder(dec);
    printf("[INFO] AUDIO UNLOADED\n");
}

void UnloadSoundAudio(unsigned char* data, Decoder *dec, NodeSource* node){
    free(data);
    UnloadAudioFile(dec, node);
}

void MasterVolume(double vol){
    ma_device_set_master_volume(&master.device, (float)vol);
}
