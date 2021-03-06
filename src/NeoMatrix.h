#ifndef _NEO_MATRIX_H
#define _NEO_MATRIX_H

#include "Arduino.h"
#include "FastLED.h"

template <uint16_t width, uint16_t height>
class Matrix
{
public:
	Matrix(CFastLED &userFastLed, CRGB *userLeds)
	{
		fastled = &userFastLed;
		leds = userLeds;
	}

	uint16_t getPixelNumber(int8_t x, int8_t y)
	{
		if ((y & 1) == 0) // y & 1 == y % 2
			return y * width + x;
		else
			return y * width + width - x - 1;
	}

	void setPixel(int8_t x, int8_t y, CRGB col)
	{
		leds[this->getPixelNumber(x, y)] = col;
	}

	CRGB getPixelColor(int8_t x, int8_t y)
	{
		return leds[this->getPixelNumber(x, y)];
	}

	void fillAll(CRGB col)
	{
		for (uint16_t i = 0; i < width * height; i++)
			leds[i] = col;
		this->fastled->show();
	}
	
private:
	CFastLED *fastled;
	CRGB *leds;
};

#endif