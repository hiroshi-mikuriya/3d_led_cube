import os
from ctypes import *
import sys
import platform

dirname = os.path.dirname(__file__)
if sys.platform == 'darwin':
    ledlib = dirname + '/libledLib.dylib'
elif sys.platform == 'win32':
    arch = platform.architecture()[0]
    if arch == '64bit':
        ledlib = dirname + '/ledLib64.dll'
    else:
        ledlib = dirname + '/ledLib32.dll'
else:
    raise NotImplementedError('Unsupported OS.' + sys.platform)

print('LoadLibrary: '+ ledlib)
led = cdll.LoadLibrary(ledlib)
