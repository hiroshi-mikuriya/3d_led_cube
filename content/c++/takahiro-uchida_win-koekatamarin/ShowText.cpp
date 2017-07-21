#include "ShowText.h"
#include "loadLibrary.hpp"
#include "GdiplusInitializer.hpp"
#include "BitmapLockWrapper.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <atlstr.h> 

#pragma comment(lib, "gdiplus.lib")

namespace
{
	void DrawChar(Gdiplus::Bitmap * canvas, WCHAR chr)
	{
		Gdiplus::Graphics g(canvas);
		Gdiplus::Font font(L"ƒƒCƒŠƒI", canvas->GetWidth() / 2);
		Gdiplus::SolidBrush brush(Gdiplus::Color(0xFF, 0xFF, 0xFF));
		Gdiplus::RectF rc(0, 0, canvas->GetWidth(), canvas->GetHeight());
		Gdiplus::StringFormat format;
		g.DrawString(CString(chr), canvas->GetWidth(), &font, rc, &format, &brush);
	}

	void SetCharFromBitmap(int x, int y, int z, BitmapLockWrapper * lock)
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
				SetLed(x2, y2, z, v);
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
	int limit = -(LED_WIDTH * imgs.size());
	for (int y = LED_HEIGHT; limit < y; --y){
		Clear();
		for (size_t i = 0; i < imgs.size(); ++i){
			int y2 = y + i * LED_WIDTH;
			SetCharFromBitmap(0, y2, 0, imgs[i].second.get());
		}
		Show();
		Wait(30);
	}
}
