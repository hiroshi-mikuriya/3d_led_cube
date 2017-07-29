#pragma once

#include <gdiplus.h>

class BitmapLockWrapper
{
	Gdiplus::BitmapData m;
	Gdiplus::Bitmap * m_bmp;
public:
	BitmapLockWrapper(Gdiplus::Bitmap * bmp)
		: m_bmp(bmp)
	{
		Gdiplus::Rect rc(0, 0, m_bmp->GetWidth(), m_bmp->GetHeight());
		if (Gdiplus::Ok != m_bmp->LockBits(&rc, 0, m_bmp->GetPixelFormat(), &m)){
			throw std::runtime_error("Failed Gdiplus::Bitmap::LockBits.");
		}
	}
	~BitmapLockWrapper()
	{
		if (Gdiplus::Ok != m_bmp->UnlockBits(&m)){
			throw std::runtime_error("Failed Gdiplus::Bitmap::UnlockBits.");
		}
	}
	Gdiplus::BitmapData const * Get()const
	{
		return &m;
	}
};