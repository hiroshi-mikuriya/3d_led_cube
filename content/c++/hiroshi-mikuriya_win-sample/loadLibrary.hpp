#pragma once

#include "led.h"
#include <stdexcept>
#include <iostream>
#include <Windows.h>

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
    HMODULE mod = LoadLibraryA(lib);
    if(0 == mod){
        throw std::runtime_error("Failed to load library");
    }
    SetUrl = reinterpret_cast<SetUrl_t*>(GetProcAddress(mod, "SetUrl"));
    SetLed = reinterpret_cast<SetLed_t*>(GetProcAddress(mod, "SetLed"));
    Clear = reinterpret_cast<Clear_t*>(GetProcAddress(mod, "Clear"));
    Show = reinterpret_cast<Show_t*>(GetProcAddress(mod, "Show"));
    Wait = reinterpret_cast<Wait_t*>(GetProcAddress(mod, "Wait"));
    SetChar = reinterpret_cast<SetChar_t*>(GetProcAddress(mod, "SetChar"));
    ShowMotioningText1 = reinterpret_cast<ShowMotioningText1_t*>(GetProcAddress(mod, "ShowMotioningText1"));
    ShowFirework = reinterpret_cast<ShowFirework_t*>(GetProcAddress(mod, "ShowFirework"));
}
