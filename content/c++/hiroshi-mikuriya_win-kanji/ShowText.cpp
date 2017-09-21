#include "ShowText.h"
#include "loadLibrary.hpp"
#include "GdiplusInitializer.hpp"
#include "BitmapLockWrapper.hpp"
#include <vector>
#include <memory>
# include <random>

#pragma comment(lib, "gdiplus.lib")

namespace
{
	int newColor()
	{
		auto gem = [](double a) -> double {
			while (6 < a) a -= 6.0;
			if (a < 1) return a;
			if (a < 3) return 1;
			if (a < 4) return 4 - a;
			return 0;
		};
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> score(0, 6.0);
		double a = score(mt);
		byte r = static_cast<byte>(gem(a + 0) * 255);
		byte g = static_cast<byte>(gem(a + 2) * 255);
		byte b = static_cast<byte>(gem(a + 4) * 255);
		return r * 0x10000 + g * 0x100 + b;
	}

	int modBrightness(int rgb, byte v)
	{
		byte r = ((rgb >> 16) & 0xFF) * v / 0xFF;
		byte g = ((rgb >> 8) & 0xFF) * v / 0xFF;
		byte b = ((rgb >> 0) & 0xFF) * v / 0xFF;
		return r * 0x10000 + g * 0x100 + b;
	}

	void DrawChar(Gdiplus::Bitmap * canvas, WCHAR chr)
	{
		Gdiplus::Graphics g(canvas);
		Gdiplus::Font font(L"ƒƒCƒŠƒI", canvas->GetWidth() / 2);
		Gdiplus::SolidBrush brush(Gdiplus::Color(0xFF, 0xFF, 0xFF));
		Gdiplus::RectF rc(0, 0, canvas->GetWidth(), canvas->GetHeight());
		Gdiplus::StringFormat format;
		g.DrawString(CString(chr), canvas->GetWidth(), &font, rc, &format, &brush);
	}

	void SetCharFromBitmap(int x, int y, int z, int rgb, BitmapLockWrapper * lock)
	{
		if (x <= -LED_WIDTH || LED_WIDTH <= x){
			return;
		}
		if (y <= -LED_WIDTH || LED_HEIGHT <= y){
			return;
		}
		if (z < 0 || LED_DEPTH <= z){
			return;
		}
		for (int xx = 0; xx < LED_WIDTH; ++xx){
			for (int yy = 0; yy < LED_WIDTH; ++yy){
				int x2 = x + xx;
				int y2 = y + yy;
				byte v = *(static_cast<byte*>(lock->Get()->Scan0) + xx * 3 + yy * lock->Get()->Stride);
				SetLed(x2, y2, z, modBrightness(rgb, v));
			}
		}
	}
}

void Initialize(const char * dll, int argc, const char* argv[])
{
	static GdiplusInitializer init;
	loadLibrary(dll);
	if (1 < argc){
		SetUrl(argv[1]);
	}
}

void ShowText(CStringW const & text)
{
	typedef std::shared_ptr<Gdiplus::Bitmap> BitmapPtr;
	typedef std::shared_ptr<BitmapLockWrapper> LockPtr;
	auto imgs = [text](){
		std::vector<std::pair<BitmapPtr, LockPtr>> dst;
		for (int i = 0; i < text.GetLength(); ++i){
			BitmapPtr bmp = std::make_shared<Gdiplus::Bitmap>(LED_WIDTH, LED_WIDTH, PixelFormat24bppRGB);
			DrawChar(bmp.get(), text[i]);
			LockPtr lock = std::make_shared<BitmapLockWrapper>(bmp.get());
			dst.push_back({ bmp, lock });
		}
		return dst;
	}();
	int rgb = newColor();
	int limit = -(LED_WIDTH * imgs.size());
	for (int y = LED_HEIGHT; limit < y; --y){
		Clear();
		for (size_t i = 0; i < imgs.size(); ++i){
			int y2 = y + i * LED_WIDTH;
			SetCharFromBitmap(0, y2, rand() % 2, rgb, imgs[i].second.get());
		}
		Show();
		Wait(30);
	}
}
