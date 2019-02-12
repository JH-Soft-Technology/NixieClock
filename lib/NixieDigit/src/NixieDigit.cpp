/**
 * NixieDigit.cpp - Library for handling led lights on each segment
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
*/
#include <Adafruit_NeoPixel.h>
#include "NixieDigit.h"

/**
 * Constructor
 * @param dataPin - pin on which data is comming to the class
 * @param numberOfSegments - number of segmets we use for our clock
 */
NixieDigit::NixieDigit(uint8_t dataPin, uint8_t numberOfSegments)
{
    pixels = Adafruit_NeoPixel(numberOfSegments * 20, dataPin, NEO_GRB + NEO_KHZ800);
    pixels.setBrightness(100);
}

/**
 * Control two segments for hour / minutes / seconds
 * @param digit - 0 / 2 / 4 means heours / minutes / seconds
 * @param value - number which should be show
 */
void NixieDigit::Digit(uint8_t digit, uint8_t value)
{
    uint8_t number = (digit * 20) + (value * 2);

    pixels.setPixelColor(number, pixels.Color(color.red, color.green, color.blue));
    pixels.setPixelColor(number + 1, pixels.Color(color.red, color.green, color.blue));
}

/**
 * turn on appropriate leds
 * @param value - number of time to show up
 * @param pos - number position 0 / 2 / 4 - means segments 0|1 for hours / 2|3 for minutes / 4|5 for seconds
 */
void NixieDigit::ShowNumber(uint16_t value, uint8_t pos)
{
    if (value >= 10)
    {
        Digit(pos, (value / 10) % 10);
        pos++;
    }
    else
    {
        Digit(pos, 0 % 10); // leading zero number
        pos++;
    }

    Digit(pos, value % 10);
}

/**
 * Every change we need to clear Adafruit_NeoPixel
 */
void NixieDigit::Clear()
{
    pixels.clear();
}

/**
 * Every change we need to show Adafruit_NeoPixel
 */
void NixieDigit::Show()
{
    pixels.show();
}

/**
 *  Start Adafruit_NeoPixel
 */
void NixieDigit::Begin()
{
    pixels.begin();
}

/**
 * Set color
 * @param red - Red color
 * @param green - Green color
 * @param blue - Blue color
 */
void NixieDigit::Color(uint8_t red, uint8_t green, uint8_t blue)
{
    color.red = red;
    color.green = green;
    color.blue = blue;
}

/**
 * Set clock brightness
 * @param brightness 0 - 250
 */
void NixieDigit::Brightness(uint8_t brightness)
{
    pixels.setBrightness(brightness);
}