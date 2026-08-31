#pragma once
#include "miniaudio.h"

typedef ma_decoder Decoder;
typedef ma_data_source_node NodeSource;

struct Master{
    ma_format format = ma_format_f32;
    int channels = 2;
    int sampleRate = 48000;
    double master_vol;

    ma_device device;
    ma_node_graph ngraph;
};

class amk{
public:
    static Master master;
    static bool initAudioDevice();
    static void endAudioDevice();

    static bool initDecoder(const char* path, Decoder& dec);
    static bool getSourceData(NodeSource& node, Decoder& dec);
    static bool sendOutput(NodeSource& node);
    static void unloadDecoder(Decoder& dec);
    static void unloadSourceData(NodeSource& node);

    static void LoadAudioFile(const char* path, Decoder& decoder, NodeSource& source);
    static void UnloadAudio(Decoder& decoder, NodeSource& node);

    static void PlayAudioFile(Decoder& dec, NodeSource& node);
    static void MasterVolume(double vol);
};
