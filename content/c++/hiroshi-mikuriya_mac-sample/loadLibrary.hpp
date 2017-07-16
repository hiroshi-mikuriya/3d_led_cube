#pragma once

#include "led.h"
#include <dlfcn.h>
#include <stdexcept>
#include <iostream>

SetUrl_t * SetUrl;
SetLed_t * SetLed;
Clear_t * Clear;
Show_t * Show;
Wait_t * Wait;
SetChar_t * SetChar;
ShowMotioningText1_t * ShowMotioningText1;
ShowFirework_t * ShowFirework;

inline void loadLibrary(const char * lib)
{
    void * dylib = dlopen(lib, RTLD_LAZY);
    if(0 == dylib){
        throw std::runtime_error(dlerror());
    }
    SetUrl = (SetUrl_t*)dlsym(dylib, "SetUrl");
    SetLed = (SetLed_t*)dlsym(dylib, "SetLed");
    Clear = (Clear_t*)dlsym(dylib, "Clear");
    Show = (Show_t*)dlsym(dylib, "Show");
    Wait = (Wait_t*)dlsym(dylib, "Wait");
    SetChar = (SetChar_t*)dlsym(dylib, "SetChar");
    ShowMotioningText1 = (ShowMotioningText1_t*)dlsym(dylib, "ShowMotioningText1");
    ShowFirework = (ShowFirework_t*)dlsym(dylib, "ShowFirework");
}
