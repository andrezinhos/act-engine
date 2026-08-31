#include "amk.hpp"
#include <cstdio>
#include "miniaudio.h"

Master amk::master = {};

void callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    Master* master = static_cast<Master*>(pDevice->pUserData);

    ma_node_graph_read_pcm_frames(&master->ngraph, pOutput, frameCount, nullptr);
}

bool amk::initAudioDevice(){
    ma_node_graph_config graph_config = ma_node_graph_config_init(master.channels);

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = master.format;
    config.playback.channels = master.channels;
    config.sampleRate = master.sampleRate;
    config.dataCallback = callback;
    config.pUserData = &master;

    if (ma_node_graph_init(&graph_config, nullptr, &master.ngraph) != MA_SUCCESS) {
        printf("[INFO] ERROR TO INTIALIZE AUDIO (NODE GRAPH)");
        return false;
    }
    if (ma_device_init(nullptr, &config, &master.device) != MA_SUCCESS){
        printf("[INFO] ERROR TO INTIALIZE AUDIO (DEVICE)");
        return false;
    }

    ma_result result = ma_device_start(&master.device);
    if (result != MA_SUCCESS){
        printf("[INFO] ERROR TO INTIALIZE AUDIO (START DEVICE)");
        return false;
    }

    printf("[INFO] AUDIO DEVICE STARTED\n");
    return true;
}

void amk::endAudioDevice(){
    ma_device_uninit(&master.device);
    ma_node_graph_uninit(&master.ngraph, nullptr);
    printf("[INFO] AUDIO DEVICE CLOSED\n");
}

void amk::LoadAudioFile(const char* path, Decoder& dec, NodeSource& node){
    if (!initDecoder(path, dec)) return;
    if (!getSourceData(node, dec)) return;

    if (!sendOutput(node)){
        unloadSourceData(node);
        unloadDecoder(dec);
        return;
    }

    printf("[INFO] AUDIO FILE LOADED");
}

void amk::PlayAudioFile(Decoder& dec, NodeSource &node){
    ma_data_source_seek_to_pcm_frame(&dec, 0);
    ma_node_set_state(&node, ma_node_state_started);
}

void amk::UnloadAudio(Decoder &dec, NodeSource& node){
    unloadSourceData(node);
    unloadDecoder(dec);
    printf("[INFO] AUDIO UNLOADED\n");
}

void amk::MasterVolume(double vol){
    master.master_vol = vol;
    ma_node_set_output_bus_volume(
        ma_node_graph_get_endpoint(&master.ngraph),
        0,
        master.master_vol
    );
}
