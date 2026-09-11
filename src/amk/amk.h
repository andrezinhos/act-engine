#pragma once
#include "miniaudio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef ma_decoder Decoder;
typedef ma_data_source_node NodeSource;

typedef struct {
    ma_format format;
    int channels;
    int sampleRate;
    double master_vol;

    ma_device device;
    ma_node_graph ngraph;
} Master;

#ifdef __cplusplus
extern "C" {
#endif

extern Master master;
bool initAudioDevice();
void endAudioDevice();

unsigned char* loadBytes(const char* path, size_t* size);

bool initDecoder(const void* data, size_t size, Decoder* dec);
bool initDecoderPath(const char* path, Decoder* dec);
bool getSourceData(NodeSource* node, Decoder* dec);
bool sendOutput(NodeSource* node);
void unloadDecoder(Decoder* dec);
void unloadSourceData(NodeSource* node);

void LoadSoundAudioFile(unsigned char* data, size_t size, Decoder* decoder, NodeSource* source);
void LoadMusicAudioFile(const char* path, Decoder* decoder, NodeSource* source);
void UnloadAudioFile(Decoder* decoder, NodeSource* node);

void UnloadSoundAudio(unsigned char* data, Decoder* decoder, NodeSource* node);

void PlayAudioFile(Decoder* dec, NodeSource* node);
void PauseAudioFile(NodeSource* node);
void ResumeAudioFile(NodeSource* node);
void StopAudioFile(Decoder* dec, NodeSource* node);

void MasterVolume(double vol);

#ifdef __cplusplus
}
#endif
