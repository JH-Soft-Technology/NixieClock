#include "RTClib.h"
#include "lib/NixieDigit/src/NixieDigit.h"
#include "lib/RotaryEncoder/src/RotaryEncoder.h"

int UP = 2;
int DOWN = 3;
int PUSH = 4;
uint8_t PIN = 7;
uint8_t DIGITS = 6;

RTC_DS3231 rtc;
NixieDigit digit = NixieDigit(PIN, DIGITS);
RotaryEncoder encoder = RotaryEncoder(UP, DOWN, PUSH);

/**
 * Write the clock to the segments
*/
void writeClock()
{
    DateTime t = rtc.now();

    digit.Clear();
    digit.ShowNumber(t.hour(), 0);
    digit.ShowNumber(t.minute(), 2);
    digit.ShowNumber(t.second(), 4);
    digit.Show();
}

/**
 * Setup the sketch
 */
void setup()
{
    Serial.begin(115200);

    attachInterrupt(0, encoder.ServiceInterrupt(), CHANGE);
    attachInterrupt(1, encoder.ServiceInterrupt(), CHANGE);

    digit.Color(170, 85, 133);
    digit.Begin();
    // start RTC
    rtc.begin();
}

void loop()
{
    encoder.Read();

    if (encoder.SettingsState == 0)
    {
        writeClock();
    }
    else
    {
    }

    // software debouncing of rotary encoder
    delay(60);
}
