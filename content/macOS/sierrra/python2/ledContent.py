from ctypes import *

led = cdll.LoadLibrary("libledLib.dylib")

for x in range(16):
    for y in range(32):
        for z in range(8):
            led.SetLed(x, y, z, 0xFF00FF)
            led.Show()
            led.Wait(20)
