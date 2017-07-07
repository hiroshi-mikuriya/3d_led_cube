#include "base64.hpp"
#include "port.h"
#include "led.h"
#include "show.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

#if defined(WIN32) || defined(WIN64) || defined(__APPLE__)
#else // Raspberry Pi
#include "spi.h"
#endif

using namespace makerfaire::fxat;

int main(int argc, const char * argv[]) {

	uint8_t lut[256] = { 0 };
	double gamma = 0.6;
	for (int ix = 0; ix < 256; ++ix){
		lut[ix] = round(255 * pow(ix / 255.0, 1.0 / gamma));
	}
	namespace asio = boost::asio;
	namespace ip = asio::ip;
	asio::io_service io_service;
	ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), static_cast<ushort>(makerfaire::fxat::Port)));
	std::cout << "Port:" << makerfaire::fxat::Port << std::endl;
	for (;;){
		ip::tcp::iostream s;
		acceptor.accept(*s.rdbuf());
		std::string msg;
		std::stringstream ss;
		decode(s, ss);
		if (ss.str().size() != Led::Width*Led::Height*Led::Depth * 3) {
			std::cerr << "size error : " << ss.str().size() << "\n" << ss.str() << std::endl;
			continue;
		}
		int m[Led::Width][Led::Height][Led::Depth] = { 0 };
		for (int x = 0; x < Led::Width; ++x){
			for (int y = 0; y < Led::Height; ++y){
				for (int z = 0; z < Led::Depth; ++z){
					char r, g, b;
					ss.get(r);
					ss.get(g);
					ss.get(b);
					m[x][y][z] = (lut[(std::uint8_t)r] << 16) + (lut[(std::uint8_t)g] << 8) + (lut[(std::uint8_t)b] << 0);
				}
			}
		}
#if defined(WIN32) || defined(WIN64) || defined(__APPLE__)
		::ShowWindow("Receiver", m);
#else // Respberry Pi
		SendSpi(m);
#endif
	}
	return 0;
}
