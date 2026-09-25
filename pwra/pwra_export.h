#pragma once

#ifdef _WIN32
    #ifdef PWRA_SL
        #define PWRAPI __declspec(dllexport)
    #else
        #define PWRAPI __declspec(dllimport)
    #endif
#else
    #define PWRAPI __attribute__ ((visibility ("default")))
#endif
