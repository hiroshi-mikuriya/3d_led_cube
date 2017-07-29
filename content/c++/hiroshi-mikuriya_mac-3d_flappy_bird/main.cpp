#include "loadLibrary.hpp"
#include <cstdlib>
#include <cmath>

int main(int argc, const char* argv[])
{
    static_cast<void>(argc); // unused
    static_cast<void>(argv); // unused
#if defined(WIN32) || defined(WIN64)
    loadLibrary("ledLib.dll");
#else if defined(__APPLE__)
    loadLibrary("./libledLib.dylib");
#endif
    if (1 < argc){
        SetUrl(argv[1]);
    }
    return 0;
}
