#include "loadLibrary.hpp"
#include <cstdlib>
#include <cmath>

struct coordinate {
	int x;
	int y;
	int z;
};

#define ON_LEDNUM 6;
struct coordinate charactor_T[6] = {
	/* x, y, z */
	{ 0, 0, 0 },
	{ 1, 0, 0 },
	{ 2, 0, 0 },
	{ 1, 0, 0 },
	{ 1, 1, 0 },
	{ 1, 2, 0 },
};

#define ON_LEDNUM_Lkakko = 3;
struct coordinate charactor_Lkakko[3] = {
	/* x, y, z */
	{ 1, 0, 0 },
	{ 0, 1, 0 },
	{ 1, 2, 0 },
};

#define ON_LEDNUM_Rkakko = 3;
struct coordinate charactor_Rkakko[3] = {
	/* x, y, z */
	{ 0, 0, 0 },
	{ 1, 1, 0 },
	{ 0, 2, 0 },
};

void dispFunc(coordinate tmp, int offsetX, int offsetY, int side);
void dispFunc2(coordinate tmp, int offsetX, int offsetY);
int main(int argc, const char* argv[])
{
	static_cast<void>(argc); // unused
	static_cast<void>(argv); // unused
#if defined(WIN32) || defined(WIN64)
	loadLibrary("ledLib32.dll");
#else if defined(__APPLE__)
	loadLibrary("./libledLib.dylib");
#endif
    if (1 < argc) {
        SetUrl(argv[1]);
    }
     int i;
	int r = 0xFF;
	int g = 0xFF / 2;
	int b = 0xFF / 2;
	int rgb;
	int x = 0, y = 0, z = 0;
	int offsetX = 0, offsetY = 0;
	int side = 0;

	rgb = (r << 16) + (g << 8) + b;
	for (;;) {
#if 0
		for (i = 0; i < 3; i++) {
			dispFunc(charactor_Lkakko[i], 0, offsetY, side);
		}

		for (i = 0; i < 6; i++) {
			dispFunc(charactor_T[i], 3, offsetY, side);
		}

		for (i = 0; i < 6; i++) {
			dispFunc(charactor_T[i], 7, offsetY, side);
		}
		for (i = 0; i < 3; i++) {
			dispFunc(charactor_Rkakko[i], 11, offsetY, side);
		}
		Show();
		Wait(50);
		Clear();
		offsetY++;
		if (offsetY > 32) {
			offsetY = 0;
			if (side == 0) {
				side = 2;
			}
			else {
				side = 0;
			}
		}
#endif

		for (i = 0; i < 3; i++) {
			dispFunc2(charactor_Lkakko[i], 0 + offsetX, offsetY);
		}

		for (i = 0; i < 6; i++) {
			dispFunc2(charactor_T[i], 3 + offsetX, offsetY);
		}

		for (i = 0; i < 6; i++) {
			dispFunc2(charactor_T[i], 7 + offsetX, offsetY);
		}
		for (i = 0; i < 3; i++) {
			dispFunc2(charactor_Rkakko[i], 11 + offsetX, offsetY);
		}
		Show();
		Wait(10);
		Clear();
		offsetX++;
		if (offsetX > 48) {
			offsetX = 0;
			offsetY = offsetY + 4;
			if (offsetY > 31) {
				offsetY = 0;
			}
		}
	}
}

void dispFunc(coordinate tmp, int offsetX, int offsetY, int side) {

	int r = 0xFF;
	int g = 0xFF / 2;
	int b = 0xFF / 2;
	int rgb;
	int x = 0, y = 0, z = 0;

	rgb = (r << 16) + (g << 8) + b;

	switch (side) {
	case 0: /* front*/
		x = tmp.x + offsetX;
		y = tmp.y + offsetY;
		z = tmp.z;
		break;
	case 1: /* right*/
		x = 15;
		y = tmp.y + offsetY;
		z = tmp.x + offsetX;
		break;
	case 2: /* back */
		x = 15 - (tmp.x + offsetX);
		y = tmp.y + offsetY;
		z = 7 - tmp.z;
	}

	/*	printf("x=%d, y=%d, z=%d\n", tmp.x + offsetX, tmp.y + offsetY, tmp.z); */
	/*	printf("x=%d, y=%d, z=%d\n", x, y, z); */

	SetLed(x, y, z, rgb);

	return;
}

void dispFunc2(coordinate tmp, int offsetX, int offsetY) {

	int r = 0xFF;
	int g = 0xFF / 2;
	int b = 0xFF / 2;
	int rgb;
	int x = 0, y = 0, z = 0;

	rgb = (r << 16) + (g << 8) + b;

	x = tmp.x + offsetX;
	y = tmp.y + offsetY;

	if (x > (2 * LED_WIDTH + 2 * LED_DEPTH)) {
		z = 0;
		x = x - 2 * 16 - 2 * 8;
	}
	else {
		if (x > (2 * LED_WIDTH + LED_DEPTH)) {
			z = 8 - (x - 2 * 16 - 8);
			x = 0;
		}
		else {
			if (x > (LED_WIDTH + LED_DEPTH)) {
				z = 0 + 7;
				x = 15 - (x - 16 - 8);
			}
			else {
				if (x > (LED_WIDTH)) {
					z = 0 + (x - 16);
					x = 15;
				}
				else {
					z = 0;
				}
			}
		}
	}

//	printf("x=%d, y=%d, z=%d\n", tmp.x + offsetX, tmp.y + offsetY, tmp.z);
//	printf("x=%d, y=%d, z=%d\n", x, y, z);

	SetLed(x, y, z, rgb);

	return;
}
