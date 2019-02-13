/**
 * NixieDigit.cpp - Library for handling led lights on each segment
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
*/
#include <Adafruit_NeoPixel.h>
#include "NixieDigit.h"
#include "lib/SegmentToNumberConvertor/src/SegmentToNumberConvertor.h"

/**
 * Constructor
 * @param dataPin - pin on which data is comming to the class
 * @param numberOfSegments - number of segmets we use for our clock
 * @param bright - led diods brightness
 */
NixieDigit::NixieDigit(uint8_t dataPin, uint8_t numberOfSegments, uint8_t bright)
{
    brightness = bright;
    _pixels = Adafruit_NeoPixel(numberOfSegments * 20, dataPin, NEO_GRB + NEO_KHZ800);
    _pixels.setBrightness(brightness * 10);
}

/**
 * Control two segments for hour / minutes / seconds
 * @param digit - 0 / 2 / 4 means heours / minutes / seconds
 * @param value - number which should be show
 */
void NixieDigit::Digit(uint8_t digit, uint8_t value)
{
    uint8_t number = (digit * 20) + (value * 2);
    _pixels.setPixelColor(number, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(number + 1, _pixels.Color(_color.red, _color.green, _color.blue));
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
 * Render time on all segments
 * @param t - DateTime time from RTC module
 */
void NixieDigit::ShowTime(DateTime t)
{
    _time = t;

    Clear();
    ShowNumber(_time.hour(), 0);
    ShowNumber(_time.minute(), 2);
    ShowNumber(_time.second(), 4);
    Show();
}

void NixieDigit::ShowColorNumber(uint8_t num)
{
    SegmentToNumberConvertor number = SegmentToNumberConvertor();

    Segments seg = number.Convert(num);

    uint8_t first = seg.array[0];
    uint8_t second = seg.array[1];
    uint8_t third = seg.array[2];

    Clear();
    _pixels.setPixelColor(first, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(first + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(second, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(second + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(third, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(third + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    Show();
}

void NixieDigit::BlinkSegment(Segment segment)
{
    Clear();

    switch (segment)
    {
    case HOURS:
        ShowNumber(_time.hour(), 0);
        break;
    case MINUTES:
        ShowNumber(_time.minute(), 2);
        break;
    case SECONDS:
        ShowNumber(_time.second(), 4);
        break;
    default:
        break;
    }

    Show();
}

/**
 * Render time on all segmenst with delay
 * @param delayTime - delay between clear and show
 * @param repeat - How many times will be repeated
 */
void NixieDigit::BlinkingAll(long delayTime, uint8_t repeat)
{
    for (int i = 0; i < repeat; i++)
    {
        Serial.print("Iteration: ");
        Serial.println(i);

        _pixels.setBrightness(0);
        ShowTime(_time);
        delay(delayTime);
        _pixels.setBrightness(brightness * 10);
        ShowTime(_time);
    }
}

/**
 * Every change we need to clear Adafruit_NeoPixel
 */
void NixieDigit::Clear()
{
    _pixels.clear();
}

/**
 * Every change we need to show Adafruit_NeoPixel
 */
void NixieDigit::Show()
{
    _pixels.show();
}

/**
 *  Start Adafruit_NeoPixel
 */
void NixieDigit::Begin()
{
    _pixels.begin();
}

/**
 * Set color
 * @param red - Red color
 * @param green - Green color
 * @param blue - Blue color
 */
RGB NixieDigit::SetColor(uint8_t red, uint8_t green, uint8_t blue)
{
    _color.red = red;
    _color.green = green;
    _color.blue = blue;

    return _color;
}

/**
 * Set clock brightness
 * @param value - from 0 to 255
 */
void NixieDigit::SetBrightness(uint8_t value)
{
    brightness = value;
    _pixels.setBrightness(brightness * 10);
    ShowTime(_time);
}

/**
 * Get current color of leds
 */
RGB NixieDigit::GetColor()
{
    return _color;
}