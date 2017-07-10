#pragma once

#define LED_WIDTH  16
#define LED_HEIGHT 32
#define LED_DEPTH  8

/*!
Specify the URL of 3D LED CUBE.
If you specify a URL, the drawing data is transferred each time you call the Show function.
@param[in] url 3D LED CUBE IP ADDRESS
*/
typedef void * SetUrl_t(char const * url);

/*!
Specify the color of each LED.
Just by calling this function, drawing data is not transferred to 3D LED CUBE.
@param[in] x 0 <= x < LED_WIDTH
@param[in] y 0 <= y < LED_HEIGHT
@param[in] z 0 <= z < LED_DEPTH
@param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
*/
typedef void * SetLed_t(int x, int y, int z, int rgb);

/*!
Clear all LED.
Just by calling this function, drawing data is not transferred to 3D LED CUBE.
*/
typedef void * Clear_t();

/*!
Show 3D LED simulator window and transfer color of each LED to 3D LED CUBE.
*/
typedef void * Show_t();


/*!
Wait for the specified time.
This function stores the time waiting for the previous time,
taking into consideration the processing delay.
*/
typedef void * Wait_t(int ms);

/*!
Display characters one by one
When the motion ends, it returns from the function.
@param[in] text
*/
typedef void * ShowMotioningText1_t(const char * text);

/*!
Show up and down moving text.
When the motion ends, it returns from the function.
@param[in] text
*/
// typedef void * ShowMotioningText2_t(const char * text);