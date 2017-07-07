#include "led.h"
#include "port.h"
#include "base64.hpp"
#include "show.hpp"
#include <stdexcept>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

void makerfaire::fxat::Wait(int ms)
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


void makerfaire::fxat::Led::SetUrl(std::string url)
{
	m_url = url;
}

void makerfaire::fxat::Led::SetLed(int x, int y, int z, int rgb)
{
    if (x < 0 || Width <= x){
        throw std::runtime_error("x should be during 0 to Width.");
    }
    if (y < 0 || Height <= y){
        throw std::runtime_error("y should be during 0 to Height.");
    }
    if (z < 0 || Depth <= z){
        throw std::runtime_error("z should be during 0 to Depth.");
    }
    if (rgb < 0 || 0xFFFFFF < rgb){
        throw std::runtime_error("rgb should be during 0x000000 to 0xFFFFFF");
    }
    m[x][y][z] = rgb;
}

void makerfaire::fxat::Led::Clear()
{
    for (int x = 0; x < Width; ++x){
        for (int y = 0; y < Height; ++y){
            for (int z = 0; z < Depth; ++z){
                m[x][y][z] = 0;
            }
        }
    }
}

namespace
{
	using namespace makerfaire::fxat;
	void send(std::string url, unsigned short port, int(&m)[Led::Width][Led::Height][Led::Depth])
	{
		namespace asio = boost::asio;
		namespace ip = asio::ip;
		asio::io_service io_service;
		ip::tcp::socket socket(io_service);
		socket.connect(ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(url), port));
		std::stringstream src, dst;
		for (int x = 0; x < Led::Width; ++x){
			for (int y = 0; y < Led::Height; ++y){
				for (int z = 0; z < Led::Depth; ++z){
					int v = m[x][y][z];
					char r = static_cast<char>(v >> 16);
					char g = static_cast<char>(v >> 8);
					char b = static_cast<char>(v >> 0);
					src << r << g << b;
				}
			}
		}
		std::cout << asio::write(socket, asio::buffer(encode(src, dst).str())) << std::endl;
	}
}

void makerfaire::fxat::Led::Show()
{
	try{
		if (!m_url.empty()){
			send(m_url, Port, m);
		}
		::ShowWindow("Sender", m);
	}
	catch (std::exception const & e){
		std::cerr << e.what() << std::endl;
	}
}
