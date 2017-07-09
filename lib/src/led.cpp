#include "led.h"
#include "base64.hpp"
#include "show.hpp"
#include "port.h"
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <chrono>

#if defined(WIN32) || defined(WIN64)
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
#endif // WIN32 or WIN64

namespace
{
	std::mutex sMutex;
    int m[LED_WIDTH][LED_HEIGHT][LED_DEPTH];
    std::string sUrl;

	class MutexLocker
	{
		std::mutex & m;
		MutexLocker & operator = (MutexLocker &) = delete;
	public:
		MutexLocker(std::mutex & mutex) : m(mutex){ m.lock(); }
		~MutexLocker(){ m.unlock(); }
	};
}


extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void SetUrl(char const * url)
{
    sUrl = url;
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void SetLed(int x, int y, int z, int rgb)
{
	MutexLocker locker(sMutex);
    if (x < 0 || LED_WIDTH <= x){
        return;
    }
    if (y < 0 || LED_HEIGHT <= y){
        return;
    }
    if (z < 0 || LED_DEPTH <= z){
        return;
    }
    m[x][y][z] = rgb;
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void Clear()
{
	MutexLocker locker(sMutex);
    for (int x = 0; x < LED_WIDTH; ++x){
        for (int y = 0; y < LED_HEIGHT; ++y){
            for (int z = 0; z < LED_DEPTH; ++z){
                m[x][y][z] = 0;
            }
        }
    }
}

namespace {
    void send(std::string url, unsigned short port, int(&m)[LED_WIDTH][LED_HEIGHT][LED_DEPTH])
    {
        namespace asio = boost::asio;
        namespace ip = asio::ip;
        asio::io_service io_service;
        ip::tcp::socket socket(io_service);
        socket.connect(ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(url), port));
        std::stringstream src, dst;
        for (int x = 0; x < LED_WIDTH; ++x){
            for (int y = 0; y < LED_HEIGHT; ++y){
                for (int z = 0; z < LED_DEPTH; ++z){
                    int v = m[x][y][z];
                    char r = static_cast<char>(v >> 16);
                    char g = static_cast<char>(v >> 8);
                    char b = static_cast<char>(v >> 0);
                    src << r << g << b;
                }
            }
        }
        std::cout << asio::write(socket, asio::buffer(makerfaire::fxat::encode(src, dst).str())) << std::endl;
    }
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void Show()
{
	MutexLocker locker(sMutex);
    try{
        if (!sUrl.empty()){
            send(sUrl, makerfaire::fxat::Port, m);
        }
        ShowWindow("Sender", m);
    }
    catch (std::exception const & e){
        std::cerr << e.what() << std::endl;
    }
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void Wait(int ms)
{
	static auto sLastWaitTime = std::chrono::system_clock::now();
	auto now = std::chrono::system_clock::now();
	int diff = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(now - sLastWaitTime).count());
	ms = std::max(1, ms - diff);
#ifdef _WIN32
    Sleep(ms);
#else
    cv::waitKey(ms);
#endif // _WIN32
	sLastWaitTime = std::chrono::system_clock::now();
}
