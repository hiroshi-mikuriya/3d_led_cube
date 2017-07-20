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
		Gdiplus::Font font(L"メイリオ", canvas->GetWidth() / 2);
		Gdiplus::SolidBrush brush(Gdiplus::Color(0xFF, 0xFF, 0xFF));
		Gdiplus::RectF rc(0, 0, canvas->GetWidth(), canvas->GetHeight());
		Gdiplus::StringFormat format;
		g.DrawString(CString(chr), canvas->GetWidth(), &font, rc, &format, &brush);
	}

	void SetChar(int x, int y, int z, BitmapLockWrapper * lock)
	{
		for (int xx = 0; xx < LED_WIDTH; ++xx){
			for (int yy = 0; yy < LED_WIDTH; ++yy){
				int x2 = x + xx;
				int y2 = y + yy;
				byte v = *(static_cast<byte*>(lock->Get()->Scan0) + xx * 3 + yy * lock->Get()->Stride);
				SetLed(x2, y2, z, v);
			}
		}
	}

	void ShowText(CString const & text)
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
		for (size_t i = 0; i < imgs.size(); ++i){
			Clear();
			SetChar(0, 0, 0, imgs[i].second.get());
			Show();
			Wait(1000);
		}
	}
}

int main(int argc, const char* argv[])
{
    static_cast<void>(argc); // unused
    static_cast<void>(argv); // unused
	loadLibrary("ledLib32.dll");
	GdiplusInitializer gdiInit;
	if (1 < argc){
		SetUrl(argv[1]);
	}
	for (;;) {
		CString text(_T("オレの名前は内田だ"));
		ShowText(text);
		exit(0);
    }
}
