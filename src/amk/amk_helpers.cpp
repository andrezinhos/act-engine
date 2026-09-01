#include "amk.hpp"
#include <fstream>
#include <vector>
#include "stb_vorbis.c"
#undef L
#undef R
#undef C
#define MA_NO_MP3
#define MA_NO_FLAC
#define MINIAUDIO_IMPLEMENTATION
#define MA_SUPPORT_VORBIS
#include "miniaudio.h"

std::vector<unsigned char> amk::loadBytes(const char* path){
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) return {};

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

    file.close();
    return buffer;
}

bool amk::initDecoder(const void* data, size_t size, Decoder& dec){
    ma_decoder_config config = ma_decoder_config_init(
        master.format,
        master.channels,
        master.sampleRate
    );

    ma_result result = ma_decoder_init_memory(data, size, &config, &dec);

    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO LOAD AUDIO FILE (DECODER)\n");
        return false;
    }

    return true;
}

bool amk::initDecoder(const char* path, Decoder& dec){
    ma_decoder_config config = ma_decoder_config_init(
        master.format,
        master.channels,
        master.sampleRate
    );

    ma_result result = ma_decoder_init_file(path, &config, &dec);
    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO LOAD AUDIO FILE (DECODER)\n");
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
        printf("[INFO] FAILED TO GET AUDIO DATA (SOURCE NODE)\n");
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
        printf("[INFO] FAILED TO LOAD AUDIO FILE (OUTPUT BUS)\n");
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
