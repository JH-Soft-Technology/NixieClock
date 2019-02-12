/**
 * NixieDigit.h - Library for handling led lights on each segment
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
 */
#ifndef NixieDigit_h
#define NixieDigit_h

#include <Adafruit_NeoPixel.h>

class NixieDigit
{
  public:
    /**
     * Structure for color handling
     */
    struct RGB
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
    NixieDigit(uint8_t dataPin, uint8_t numberOfSegments);
    void ShowNumber(uint16_t value, uint8_t pos);
    void Color(uint8_t red, uint8_t green, uint8_t blue);
    void Clear();
    void Show();
    void Begin();
    void Brightness(uint8_t brightness);

  private:
    RGB color;
    Adafruit_NeoPixel pixels;
    void Digit(uint8_t digit, uint8_t value);
};

#endif