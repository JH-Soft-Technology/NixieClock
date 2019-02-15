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
 * @param bright - led diods brightness It is conversion of 0 - 250 range. It has 25 position on rotary encoder
 *                 each state is multiplied by 10.
 */
NixieDigit::NixieDigit(uint8_t dataPin, uint8_t numberOfSegments, uint8_t bright)
{
    brightness = bright;
    _pixels = Adafruit_NeoPixel(numberOfSegments * 20, dataPin, NEO_GRB + NEO_KHZ800);
    _pixels.setBrightness(brightness * 10);
}

/**
 * Render time on all segments
 * @param t - DateTime time from RTC module
 */
void NixieDigit::ShowTime(DateTime t)
{
    _time = t;

    Clear();
    _ShowNumber(_time.hour(), 0);
    _ShowNumber(_time.minute(), 2);
    _ShowNumber(_time.second(), 4);
    Show();
}

/**
 * Shows number represented in decimal format on first three segments
 * It is used for RGB setup or Time setup.
 * Converts a decimal number of the input to the led position on the  first
 * three segments. Hundreds on the first, dozens on the second and units on the third
 * @param num - can bee decimal number in the range of 0 - 999
 */
void NixieDigit::ShowNumber(uint8_t num)
{
    // class with segment setup
    SegmentToNumberConvertor number = SegmentToNumberConvertor();
    // convert number to three segments
    Segments seg = number.Convert(num);

    uint8_t first = seg.array[0];
    uint8_t second = seg.array[1];
    uint8_t third = seg.array[2];

    // set up the segments with numbers
    Clear();
    _pixels.setPixelColor(first, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(first + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(second, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(second + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(third, _pixels.Color(_color.red, _color.green, _color.blue));
    _pixels.setPixelColor(third + 1, _pixels.Color(_color.red, _color.green, _color.blue));
    Show();
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
void NixieDigit::SetColor(uint8_t red, uint8_t green, uint8_t blue)
{
    _color.red = red;
    _color.green = green;
    _color.blue = blue;
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
 * Set each part of time (Hours, Minutes, Seconds)
 * @param value - number which will be set to the RTC
 * @param rtc - object of the RTC
 * @param segment - represents segment which is going to be setup
 */
void NixieDigit::SetTime(uint8_t value, RTC_DS3231 &rtc, Segment segment)
{
    _time = rtc.now();

    if (segment == HOURS)
    {
        _time = DateTime(_time.year(), _time.month(), _time.day(), value, _time.minute(), _time.second());
    }
    else if (segment == MINUTES)
    {
        _time = DateTime(_time.year(), _time.month(), _time.day(), _time.hour(), value, _time.second());
    }
    else if (segment == SECONDS)
    {
        _time = DateTime(_time.year(), _time.month(), _time.day(), _time.hour(), _time.minute(), value);
    }

    rtc.adjust(_time);
}

/**
 * Get current color of leds when the
 * setting has been started
 */
RGB NixieDigit::GetColor()
{
    return _color;
}

/**
 * Get the inner snapshot of the time when
 * the settings has been started
 */
DateTime NixieDigit::GetTime()
{
    return _time;
}

/**
 * Control two segments for hour / minutes / seconds
 * @param digit - 0 / 2 / 4 means heours / minutes / seconds
 * @param value - number which should be show
 */
void NixieDigit::_Digit(uint8_t digit, uint8_t value)
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
void NixieDigit::_ShowNumber(uint16_t value, uint8_t pos)
{
    if (value >= 10)
    {
        _Digit(pos, (value / 10) % 10);
        pos++;
    }
    else
    {
        _Digit(pos, 0 % 10); // leading zero number
        pos++;
    }

    _Digit(pos, value % 10);
}