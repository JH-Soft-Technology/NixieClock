#include "RTClib.h"
#include "lib/NixieDigit/src/NixieDigit.h"
#include "lib/RotaryEncoder/src/RotaryEncoder.h"

int UP = 2;
int DOWN = 3;
int PUSH = 4;
uint8_t PIN = 7;

// 6 digits is whole clock with hours, minutes and SECONDS
// can be 4. Only hours nad minutes
// when only 4 segments will be used, it will automaticaly change
// maximum state on rotary encoder from 7 to 6
uint8_t DIGITS = 6;

RTC_DS3231 rtc;
NixieDigit digit = NixieDigit(PIN, DIGITS, 8); // startu brightness is 80 from rang 0 - 250
RotaryEncoder encoder = RotaryEncoder(UP, DOWN, PUSH, DIGITS == 6 ? 7 : 6);

/**
 * Setup the sketch
 */
void setup()
{
    Serial.begin(115200);

    attachInterrupt(0, serviceRotaryInterupt, CHANGE);
    attachInterrupt(1, serviceRotaryInterupt, CHANGE);

    // Name: Cyan Cornflower Blue
    // hex: #1D8ECE
    digit.SetColor(29, 142, 206);
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
        setBrightness();
        setColors();
        setTime();
    }
}

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
 * Set up segments brightness
 */
void setBrightness()
{
    if (encoder.state == BRIGHTNESS)
    {
        // set volume to the current brightness value
        encoder.SetVolume(digit.brightness, BRIGHTNESS);
        digit.SetBrightness(encoder.Volume);
    }
}

/**
 * Set up color of all segments by each part of RGB
 */
void setColors()
{
    RGB color;

    if (encoder.state == COLOR_RED)
    {
        color = digit.GetColor();
        encoder.SetVolume(color.red, COLOR_RED);
        digit.SetColor(encoder.Volume, color.green, color.blue);
        digit.ShowNumber(encoder.Volume);
    }
    if (encoder.state == COLOR_GREEN)
    {
        color = digit.GetColor();
        encoder.SetVolume(color.green, COLOR_GREEN);
        digit.SetColor(color.red, encoder.Volume, color.blue);
        digit.ShowNumber(encoder.Volume);
    }
    if (encoder.state == COLOR_BLUE)
    {
        color = digit.GetColor();
        encoder.SetVolume(color.blue, COLOR_BLUE);
        digit.SetColor(color.red, color.green, encoder.Volume);
        digit.ShowNumber(encoder.Volume);
    }
}

/**
 * Set up the time of the clock
 */
void setTime()
{
    DateTime timeStamp;

    if (encoder.state == TIME_HOURS)
    {
        timeStamp = digit.GetTime();
        encoder.SetVolume(timeStamp.hour(), TIME_HOURS);
        digit.SetTime(encoder.Volume, rtc, digit.HOURS);
        Serial.println(encoder.Volume);
        digit.ShowNumber(encoder.Volume);
    }
    else if (encoder.state == TIME_MINUTES)
    {
        timeStamp = digit.GetTime();
        encoder.SetVolume(timeStamp.minute(), TIME_MINUTES);
        digit.SetTime(encoder.Volume, rtc, digit.MINUTES);
        Serial.println(encoder.Volume);
        digit.ShowNumber(encoder.Volume);
    }
    else if (encoder.state == TIME_SECONDS)
    {
        timeStamp = digit.GetTime();
        encoder.SetVolume(timeStamp.second(), TIME_SECONDS);
        digit.SetTime(encoder.Volume, rtc, digit.SECONDS);
        Serial.println(encoder.Volume);
        digit.ShowNumber(encoder.Volume);
    }
}