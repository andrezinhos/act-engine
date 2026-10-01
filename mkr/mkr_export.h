#pragma once

#ifdef _WIN32
    #ifdef MKR_SL
        #define MKRAPI __declspec(dllexport)
    #else
        #define MKRAPI __declspec(dllimport)
    #endif
#else
    #define MKRAPI __attribute__ ((visibility ("default")))
#endif
