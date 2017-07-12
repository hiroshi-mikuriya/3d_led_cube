import os
from ctypes import *
from sys import platform

if platform == 'darwin':
    ledlib = './lib/libledLib.dylib'
elif platform == 'win32':
    ledlib = './lib/ledLib32.dll'
elif platform == 'win64':
    ledlib = './lib/ledLib64.dll'
else:
    raise NotImplementedError('Unsupported OS.')

led = cdll.LoadLibrary(ledlib)
