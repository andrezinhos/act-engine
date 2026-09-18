#include "amk.h"
#include "stb_vorbis.c"
#undef L
#undef R
#undef C
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_MP3
#define MA_NO_FLAC
#define MA_NO_ENGINE
#define MA_NO_RESOURCE_MANAGER
#define MA_SUPPORT_VORBIS
#include "miniaudio.h"

unsigned char* loadBytes(const char* path, size_t* size){
    FILE* file = fopen(path, "rb"); 
    
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);

    unsigned char* buffer = (unsigned char*)malloc(fsize);

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

bool initDecoder(const void* data, size_t size, Decoder* dec){
    ma_decoder_config config = ma_decoder_config_init(
        master.format,
        master.channels,
        master.sampleRate
    );

    ma_result result = ma_decoder_init_memory(data, size, &config, dec);

    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO LOAD AUDIO FILE (DECODER)\n");
        return false;
    }

    return true;
}

bool initDecoderPath(const char* path, Decoder* dec){
    ma_decoder_config config = ma_decoder_config_init(
        master.format,
        master.channels,
        master.sampleRate
    );

    ma_result result = ma_decoder_init_file(path, &config, dec);
    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO LOAD AUDIO FILE (DECODER)\n");
        return false;
    }

    return true;
}

bool getSourceData(NodeSource* node, Decoder *dec){
    ma_data_source_node_config config = ma_data_source_node_config_init(dec);

    config.nodeConfig.initialState = ma_node_state_stopped;

    ma_result result = ma_data_source_node_init(
        &master.ngraph,
        &config,
        NULL,
        node
    );

    if (result != MA_SUCCESS){
        printf("[INFO] FAILED TO GET AUDIO DATA (SOURCE NODE)\n");
        return false;
    }

    return true;
}

bool sendOutput(NodeSource *node){
    ma_result result = ma_node_attach_output_bus(
        node,
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

void unloadDecoder(Decoder *dec){
    ma_decoder_uninit(dec);
}

void unloadSourceData(NodeSource *node){
    ma_data_source_node_uninit(
        node,
        NULL
    );
}
