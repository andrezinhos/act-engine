#include "amk.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

bool amk::initDecoder(const char* path, Decoder& dec){
    ma_decoder_config config = ma_decoder_config_init(
        master.format,
        master.channels,
        master.sampleRate
    );

    ma_result result = ma_decoder_init_file(path, &config, &dec);
    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO LOAD AUDIO FILE (DECODER)");
        return false;
    }

    return true;
}

bool amk::getSourceData(NodeSource& node, Decoder &dec){
    ma_data_source_node_config config = ma_data_source_node_config_init(&dec);
    config.nodeConfig.initialState = ma_node_state_stopped;

    ma_result result = ma_data_source_node_init(
        &master.ngraph,
        &config,
        nullptr,
        &node
    );

    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO GET AUDIO DATA (SOURCE NODE)");
        return false;
    }

    return true;
}

bool amk::sendOutput(NodeSource &node){
    ma_result result = ma_node_attach_output_bus(
        &node,
        0,
        ma_node_graph_get_endpoint(&master.ngraph),
        0
    );

    if (result != MA_SUCCESS) {
        printf("[INFO] FAILED TO LOAD AUDIO FILE (OUTPUT BUS)");
        return false;
    }

    return true;
}

void amk::unloadDecoder(Decoder &dec){
    ma_decoder_uninit(&dec);
}

void amk::unloadSourceData(NodeSource &node){
    ma_data_source_node_uninit(
        &node,
        nullptr
    );
}
