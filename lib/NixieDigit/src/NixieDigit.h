/**
 * NixieDigit.h - Library for handling led lights on each segment
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
 */
#ifndef NixieDigit_h
#define NixieDigit_h

#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

/**
 * Structure for color handling
 */
struct RGB
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class NixieDigit
{
  public:
    // Segment definition
    enum Segment
    {
        HOURS,
        MINUTES,
        SECONDS,
    };
    uint8_t brightness;
    NixieDigit(uint8_t dataPin, uint8_t numberOfSegments, uint8_t brightness);
    void Begin();
    void Clear();
    void Show();
    RGB GetColor();
    DateTime GetTime();
    void SetColor(uint8_t value, uint8_t green, uint8_t blue);
    void SetBrightness(uint8_t brightness);
    void SetTime(uint8_t value, RTC_DS3231 &rtc, Segment segment);
    void ShowTime(DateTime t);
    void ShowNumber(uint8_t num);

  private:
    DateTime _time;
    RGB _color;
    Adafruit_NeoPixel _pixels;
    Segment _segment;
    void _Digit(uint8_t digit, uint8_t value);
    void _ShowNumber(uint16_t value, uint8_t pos);
};

#endif