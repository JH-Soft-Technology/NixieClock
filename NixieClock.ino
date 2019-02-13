#include "RTClib.h"
#include "lib/NixieDigit/src/NixieDigit.h"
#include "lib/RotaryEncoder/src/RotaryEncoder.h"

int UP = 2;
int DOWN = 3;
int PUSH = 4;
uint8_t PIN = 7;
uint8_t DIGITS = 6;

RTC_DS3231 rtc;
NixieDigit digit = NixieDigit(PIN, DIGITS, 8);
RotaryEncoder encoder = RotaryEncoder(UP, DOWN, PUSH);

/**
 * Write the clock to the segments
*/
void writeClock()
{
    digit.ShowTime(rtc.now());
}

/**
 * Handling serivce interupting of rotary encoder
 */
void serviceRotaryInterupt()
{
    encoder.ServiceInterrupt();
}

/**
 * Setup the sketch
 */
void setup()
{
    Serial.begin(115200);

    attachInterrupt(0, serviceRotaryInterupt, CHANGE);
    attachInterrupt(1, serviceRotaryInterupt, CHANGE);

    digit.SetColor(170, 85, 133);
    digit.Begin();
    // start RTC
    rtc.begin();
}

void loop()
{
    encoder.Read();

    if (encoder.state == NONE)
    {
        writeClock();
    }
    else
    {
        handlingBrightness();
        handlingColors();
    }
}

void handlingBrightness()
{
    if (encoder.state == BRIGHTNESS)
    {
        // set volume to the current brightness value
        encoder.SetVolume(digit.brightness, BRIGHTNESS);
        digit.SetBrightness(encoder.Volume);
    }
}

void handlingColors()
{
    RGB color;

    if (encoder.state == COLOR_RED)
    {
        Serial.print("RED: ");
        Serial.println(encoder.Volume);

        color = digit.GetColor();
        encoder.SetVolume(color.red, COLOR_RED);
        digit.SetColor(encoder.Volume, color.green, color.red);
        digit.ShowColorNumber(encoder.Volume);
    }
    if (encoder.state == COLOR_GREEN)
    {
        Serial.print("GREEN: ");
        Serial.println(encoder.Volume);

        color = digit.GetColor();
        encoder.SetVolume(color.green, COLOR_GREEN);
        digit.SetColor(color.red, encoder.Volume, color.blue);
        digit.ShowColorNumber(color.red);
        digit.ShowColorNumber(encoder.Volume);
    }
    if (encoder.state == COLOR_BLUE)
    {
        Serial.print("BLUE: ");
        Serial.println(encoder.Volume);

        color = digit.GetColor();
        encoder.SetVolume(color.blue, COLOR_BLUE);
        digit.SetColor(color.red, color.green, encoder.Volume);
        digit.ShowColorNumber(encoder.Volume);
    }
}