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
    enum Segment
    {
        HOURS,
        MINUTES,
        SECONDS,
    };
    uint8_t brightness;
    NixieDigit(uint8_t dataPin, uint8_t numberOfSegments, uint8_t brightness);
    RGB SetColor(uint8_t value, uint8_t green, uint8_t blue);
    RGB GetColor();
    void ShowTime(DateTime t);
    void ShowColorNumber(uint8_t num);
    void Clear();
    void Show();
    void Begin();
    void SetBrightness(uint8_t brightness);
    void BlinkingAll(long delayTime, uint8_t repeat);

  private:
    DateTime _time;
    RGB _color;
    Adafruit_NeoPixel _pixels;
    Segment _segment;
    void Digit(uint8_t digit, uint8_t value);
    void ShowNumber(uint16_t value, uint8_t pos);
    void BlinkSegment(Segment segment);
};

#endif