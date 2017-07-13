import os
from ctypes import *
from sys import platform
import sys

dirname = os.path.dirname(__file__)
if platform == 'darwin':
    ledlib = dirname + '/libledLib.dylib'
elif platform == 'win32':
    ledlib = dirname + '/ledLib32.dll'
elif platform == 'win64':
    ledlib = dirname + '/ledLib64.dll'
else:
    raise NotImplementedError('Unsupported OS.')

led = cdll.LoadLibrary(ledlib)
