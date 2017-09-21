#pragma once

#include <gdiplus.h>

class GdiplusInitializer
{
	ULONG_PTR m_token;
public:
	GdiplusInitializer()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&m_token, &gdiplusStartupInput, NULL);
	}
	~GdiplusInitializer()
	{
		Gdiplus::GdiplusShutdown(m_token);
	}
};