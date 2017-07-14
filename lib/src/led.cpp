#include "led.h"
#include "base64.hpp"
#include "show.hpp"
#include "port.h"
#include <opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <mutex>
#include <chrono>
#include <random>

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

	struct xyz_t{
		double x, y, z;
		double len() const { return std::sqrt(x*x + y*y + z*z); }
	};

	xyz_t xyz(double x, double y, double z)
	{
		return{ x, y, z };
	}

	struct xyzc_t{
		xyz_t p;
		int color;
	};

	xyz_t operator*(xyz_t const & a, double t)
	{
		return{ a.x*t, a.y*t, a.z*t };
	}

	xyz_t operator+(xyz_t const & a, xyz_t const & b)
	{
		return{ a.x + b.x, a.y + b.y, a.z + b.z };
	}

	xyz_t operator-(xyz_t const & a, xyz_t const & b)
	{
		return{ a.x - b.x, a.y - b.y, a.z - b.z };
	}

	xyz_t & operator+=(xyz_t & a, xyz_t const & b)
	{
		a = a + b;
		return a;
	}

	xyz_t & operator-=(xyz_t & a, xyz_t const & b)
	{
		a = a - b;
		return a;
	}

	double ip(xyz_t const & a, xyz_t const & b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	xyz_t op(xyz_t const & a, xyz_t const & b)
	{
		return{
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		};
	}


	xyz_t sphere_face(std::mt19937 & rng)
	{
		std::uniform_real_distribution<double> dist(-1, 1);
		for (;;) {
			xyz_t pos = { dist(rng), dist(rng), dist(rng) };
			double d2 = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z;
			if (0.1<d2 && d2 < 1) {
				double d = 0.5;
				return{ pos.x * d, pos.y * d + 0.1, pos.z * d };
			}
		}
	}

	bool can_show(xyz_t const & p)
	{
		return 0 <= p.x && p.x < LED_WIDTH
			&& 0 <= p.y && p.y < LED_HEIGHT
			&& 0 <= p.z && p.z < LED_DEPTH;
	}
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

namespace
{
	std::string toText(cv::Mat const & src)
	{
		std::string dst;
		for (int y = 0; y < src.rows; ++y){
			for (int x = 0; x < src.cols; ++x){
				unsigned char b = src.at<unsigned char>(y, x);
				char a = 'A' + b / 16;
				dst += a;
			}
		}
		return dst;
	}
    
    cv::Mat makeMat(char c)
    {
        char chr[2] = { c };
        int thickness = 2;
        int baseline = 0;
        auto const s = cv::getTextSize(chr, cv::FONT_HERSHEY_DUPLEX, 1, thickness, &baseline);
        cv::Mat m = cv::Mat::zeros({ s.width, s.height + baseline }, CV_8UC1);
        cv::putText(m, chr, { (m.cols - s.width) / 2, (m.rows + s.height) / 2 }, cv::FONT_HERSHEY_DUPLEX, 1, 255, thickness, CV_AA, false);
        cv::resize(m, m, { LED_WIDTH, LED_HEIGHT });
        return m;
    }
    
	std::vector<std::string> makeLetters(std::string str)
	{
		std::vector<std::string> dst;
		for (auto it = str.begin(); it != str.end(); ++it){
            dst.push_back(toText(makeMat(*it)));
		}
		return dst;
	}
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void ShowMotioningText1(const char * text)
{
	const int N0 = 4 * 2;
	const int N1 = 4 * 2;
	auto color = [&](std::string const & a, xyz_t p, int ix)->int {
		int colbase;
		int xx = p.x * 15 / LED_WIDTH;
		int yy = p.y * 15 / LED_HEIGHT;
		auto cb = [](int r, int g, int b)->int
		{
			return 0x1 * b + 0x100 * g + 0x10000 * r;
		};
		switch (ix % 3) {
		case 0:  colbase = cb(xx, yy, 15); break;
		case 1:  colbase = cb(15, 15 - xx, yy); break;
		case 2:  colbase = cb(15 - yy, 15, xx); break;
		default:
			throw "DEATH!";
		}
		return (a[p.x + p.y*LED_WIDTH] - 'A') * colbase;
	};
	auto show = [&](std::string const & a, std::string const & b, double move, double light, int ix) {
		struct pp_t{
			xyz_t p[2];
		};
		std::mt19937 rng(1);
		std::vector<pp_t> pps;
		while (pps.size()<2000) {
			xyz_t p0{
				static_cast<double>(rng() % LED_WIDTH),
				static_cast<double>(rng() % LED_HEIGHT),
				static_cast<double>(rng() % LED_DEPTH) };
			xyz_t p1{
				static_cast<double>(rng() % LED_WIDTH),
				static_cast<double>(rng() % LED_HEIGHT),
				static_cast<double>(rng() % LED_DEPTH) };
			if (color(a, p0, ix) && color(b, p1, ix + 1)) {
				pps.push_back({ { p0, p1 } });
			}
		}
		Clear();
		for (auto const & pp : pps) {
			xyz_t p = pp.p[0] * (1 - move) + pp.p[1] * move;
			int c0 = color(a, pp.p[0], ix);
			int c1 = color(b, pp.p[1], ix + 1);
			auto mean = [&](int mask, double d) {
				int v = ((c0 & mask)*(1 - move) + (c1 & mask)*move)*d;
				return v & mask;
			};
			int col = mean(0xff0000, light) + mean(0xff00, light) + mean(0xff, light);
			if (can_show(p)) {
				SetLed(p.x, p.y, p.z, col);
			}
		}
		Show();
		Wait(50);
	};
	auto fade_in = [&](std::string const & c, int letter_index) {
		for (int ix = 0; ix < N0; ++ix) {
			show(c, c, 0, ix*1.0 / N0, letter_index);
		}
	};
	auto fade_out = [&](std::string const & c, int letter_index) {
		for (int ix = 0; ix < N0; ++ix) {
			show(c, c, 0, 1 - ix*1.0 / N0, letter_index);
		}
	};
	auto stop = [&](std::string const &  c, int letter_index) {
		for (int ix = 0; ix < N0; ++ix) {
			show(c, c, 0, 1, letter_index);
		}
	};
	auto move = [&](std::string const & a, std::string const & b, int letter_index) {
		for (int ix = 0; ix < N1; ++ix) {
			show(a, b, ix*1.0 / N1, 1, letter_index);
		}
	};

    auto const letters = makeLetters(text);
	for (size_t ix = 0; ix < letters.size(); ++ix) {
		if (ix == 0) {
			fade_in(letters[ix], ix);
			stop(letters[ix], ix);
		}
		else if (ix == letters.size() - 1) {
			stop(letters[ix], ix);
			fade_out(letters[ix], ix);
		}
		else {
			stop(letters[ix], ix);
		}
		if (ix < letters.size() - 1) {
			move(letters[ix], letters[ix + 1], ix);
		}
	}
}

extern "C"
#ifdef WIN32
__declspec(dllexport)
#endif
void SetChar(int x, int y, int z, char c, int rgb)
{
    auto u = [](int rgb, int shift){
        return static_cast<unsigned char>((rgb >> shift) & 0xFF);
    };
    auto src = makeMat(c);
    for (int y2 = 0; y2 < src.rows; ++y2){
        for (int x2 = 0; x2 < src.cols; ++x2){
            unsigned char c = src.at<unsigned char>(y2, x2);
            int r = u(rgb, 16) * c / 255;
            int g = u(rgb, 8) * c / 255;
            int b = u(rgb, 0) * c / 255;
            int rgb2 = 0x1 * b + 0x100 * g + 0x10000 * r;
            int xx = x + x2;
            int yy = y + y2;
            SetLed(xx, yy, z, rgb2);
        }
    }
}
