#pragma once

#include "led.h"
#include <stdexcept>
#include <iostream>
#include <Windows.h>

#undef SetPort // SetPortWÇ∆Ç¢Ç§ä÷êîÇ™Ç†ÇÈÇÁÇµÇ¢

SetUrl_t * SetUrl;
SetPort_t * SetPort;
SetLed_t * SetLed;
Clear_t * Clear;
Show_t * Show;
Wait_t * Wait;
EnableSimulator_t * EnableSimulator;
SetChar_t * SetChar;
ShowMotioningText1_t * ShowMotioningText1;
ShowFirework_t * ShowFirework;

#define LOAD_METHOD(m) do{ m = reinterpret_cast<m##_t*>(GetProcAddress(mod, #m)); }while(0)

inline void loadLibrary(const char * lib)
{
    HMODULE mod = LoadLibraryA(lib);
    if(0 == mod){
        throw std::runtime_error("Failed to load library");
    }
    LOAD_METHOD(SetUrl);
    LOAD_METHOD(SetPort);
    LOAD_METHOD(SetLed);
    LOAD_METHOD(Clear);
    LOAD_METHOD(Show);
    LOAD_METHOD(Wait);
    LOAD_METHOD(EnableSimulator);
    LOAD_METHOD(SetChar);
    LOAD_METHOD(ShowMotioningText1);
    LOAD_METHOD(ShowFirework);
}
