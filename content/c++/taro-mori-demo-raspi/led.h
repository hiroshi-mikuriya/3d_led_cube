#pragma once

#define LED_WIDTH  16
#define LED_HEIGHT 32
#define LED_DEPTH  8

/*!
 Specify the URL of 3D LED CUBE.
 If you specify a URL, the drawing data is transferred each time you call the Show function.
 @param[in] url 3D LED CUBE IP ADDRESS
*/
void SetUrl(char const * url);

/*!
Specify the PORT of 3D LED CUBE.
If you specify a URL, the drawing data is transferred each time you call the Show function.
@param[in] port 3D LED CUBE PORT
*/
void SetPort(unsigned short port);

/*!
 Specify the color of each LED.
 Just by calling this function, drawing data is not transferred to 3D LED CUBE.
 @param[in] x 0 <= x < LED_WIDTH
 @param[in] y 0 <= y < LED_HEIGHT
 @param[in] z 0 <= z < LED_DEPTH
 @param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
*/
void SetLed(int x, int y, int z, int rgb);

/*!
 Clear all LED.
 Just by calling this function, drawing data is not transferred to 3D LED CUBE.
*/
void Clear();

/*!
 Show 3D LED simulator window and transfer color of each LED to 3D LED CUBE.
*/
void Show();


/*!
 Wait for the specified time.
 This function stores the time waiting for the previous time,
 taking into consideration the processing delay.
*/
void Wait(int ms);

/*!
 Enable or Disable a Simulator Window.
 Default setting is Enable.
 @param[in] isEnable true: Show Simulator Window, false: Hide
 */
void EnableSimulator(bool isEnable);

/*!
 Specify the color of each LED for displaying a character.
 Just by calling this function, drawing data is not transferred to 3D LED CUBE.
 @param[in] x 0 <= x < LED_WIDTH
 @param[in] y 0 <= y < LED_HEIGHT
 @param[in] z 0 <= z < LED_DEPTH
 @param[in] c charactor code
 @param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
 */
void SetChar(int x, int y, int z, char c, int rgb);

/*!
 Display characters one by one
 When the motion ends, it returns from the function.
 @param[in] text displayed charactors
*/
void ShowMotioningText1(const char * text);

/*!
 Display a firework.
 When the motion ends, it returns from the function.
 @param[in] x 0 <= x < LED_WIDTH
 @param[in] y 0 <= y < LED_HEIGHT
 @param[in] z 0 <= z < LED_DEPTH
 */
void ShowFirework(int x, int y, int z);
