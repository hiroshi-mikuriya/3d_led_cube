#pragma once
#include "../common/led.h"

void SendSpi(int const (&led)[makerfaire::fxat::Led::Width][makerfaire::fxat::Led::Height][makerfaire::fxat::Led::Depth]);
