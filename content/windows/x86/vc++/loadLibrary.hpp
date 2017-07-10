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
}
