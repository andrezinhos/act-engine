#include "stb_vorbis.c"
#undef L
#undef R
#undef C
#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_MP3
#define MA_NO_FLAC
#define MA_NO_RESOURCE_MANAGER
#define MA_NO_GENERATION
#define MA_NO_ENCODING
#define MA_NO_WEBAUDIO
#define MA_NO_DSOUND
#define MA_NO_JACK
#define MA_NO_WINMM
#define MA_NO_AUDIO4
#define MA_NO_CUSTOM
#define MA_NO_OPENSL
#define MA_NO_SNDIO
#define MA_NO_OSS

#define MA_ENABLE_WASAPI
#define MA_ENABLE_ALSA
#define MA_ENABLE_COREAUDIO

#define MA_SUPPORT_VORBIS
#include "miniaudio.h"
