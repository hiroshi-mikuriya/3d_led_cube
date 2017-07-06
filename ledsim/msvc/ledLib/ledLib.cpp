#include "led.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <chrono>

#ifdef WIN32
#ifdef _DEBUG
#define LIB_EXTENSION "d.lib"
#else
#define LIB_EXTENSION ".lib"
#endif
#define OPENCV_VERSION CVAUX_STR(CV_VERSION_EPOCH) CVAUX_STR(CV_VERSION_MAJOR) CVAUX_STR(CV_VERSION_MINOR)
#pragma comment( lib, "opencv_core" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_imgproc" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_highgui" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "zlib" LIB_EXTENSION)
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "User32.lib")
#pragma comment( lib, "Gdi32.lib")
#pragma comment( lib, "Advapi32.lib")
#endif // WIN32

namespace
{
	makerfaire::fxat::Led sLed;
	std::mutex sMutex;

	class MutexLocker
	{
		std::mutex & m;
		MutexLocker & operator = (MutexLocker &) = delete;
	public:
		MutexLocker(std::mutex & mutex) : m(mutex){ m.lock(); }
		~MutexLocker(){ m.unlock(); }
	};
}


#ifdef WIN32
extern "C"
__declspec(dllexport)
#endif
void SetUrl(char const * url)
{
	sLed.SetUrl(url);
}

#ifdef WIN32
extern "C"
__declspec(dllexport)
#endif
void SetLed(int x, int y, int z, int rgb)
{
	MutexLocker locker(sMutex);
	sLed.SetLed(x, y, z, rgb);
}

#ifdef WIN32
extern "C"
__declspec(dllexport)
#endif
void Clear()
{
	MutexLocker locker(sMutex);
	sLed.Clear();
}

#ifdef WIN32
extern "C"
__declspec(dllexport)
#endif
void Show()
{
	MutexLocker locker(sMutex);
	sLed.Show();
}

#ifdef WIN32
extern "C"
__declspec(dllexport)
#endif
void Wait(int ms)
{
	static auto sLastWaitTime = std::chrono::system_clock::now();
	auto now = std::chrono::system_clock::now();
	int diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - sLastWaitTime).count();
	ms = std::max(1, ms - diff);
	makerfaire::fxat::Wait(ms);
	sLastWaitTime = std::chrono::system_clock::now();
}
