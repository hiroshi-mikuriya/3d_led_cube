#include "loadLibrary.hpp"
#include "GdiplusInitializer.hpp"
#include "BitmapLockWrapper.hpp"
#include <atlstr.h> 

#pragma comment(lib, "gdiplus.lib")

namespace
{
	void DrawChar(Gdiplus::Bitmap * canvas, WCHAR chr)
	{
		Gdiplus::Graphics g(canvas);
		Gdiplus::Font font(L"Tahoma", canvas->GetWidth());
		Gdiplus::SolidBrush brush(Gdiplus::Color(0xFF, 0xFF, 0xFF));
		Gdiplus::RectF rc(0, 0, canvas->GetWidth(), canvas->GetHeight());
		Gdiplus::StringFormat format;
		g.DrawString(CString(chr), canvas->GetWidth(), &font, rc, &format, &brush);
	}

	void ShowText(CString const & text)
	{
		for (int i = 0; i < text.GetLength(); ++i){
			Gdiplus::Bitmap canvas(LED_WIDTH, LED_HEIGHT, PixelFormat24bppRGB);
			DrawChar(&canvas, text[i]);
			BitmapLockWrapper locker(&canvas);
			Clear();
			for (int x = 0; x < LED_WIDTH; ++x){
				for (int y = 0; y < LED_HEIGHT; ++y){
					byte v = *(static_cast<byte*>(locker.Get()->Scan0) + x * 3 + y * locker.Get()->Stride);
					SetLed(x, y, 0, v);
				}
			}
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
		CString text(_T("‚ ‚¢‚¤‚¦‚¨"));
		ShowText(text);
		exit(0);
    }
}
