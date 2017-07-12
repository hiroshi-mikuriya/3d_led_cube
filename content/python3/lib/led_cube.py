import os
from ctypes import *
from sys import platform

cd = os.path.dirname(__file__)
if platform == 'darwin':
    ledlib = cd + '/libledLib.dylib'
elif platform == 'win32':
    ledlib = cd + '/ledLib32.dll'
elif platform == 'win64':
    ledlib = cd + '/ledLib64.dll'
else:
    raise NotImplementedError('Unsupported OS.')

led = cdll.LoadLibrary(ledlib)
