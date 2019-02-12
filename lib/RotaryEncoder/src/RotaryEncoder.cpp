/**
 * RotaryEncoder.h - Library for handling rotary encoder to setup the clock
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
 */

#include "wiring_private.h"
#include "RotaryEncoder.h"

/**
 * Constructor of RotaryEncoder
 * @param up - move from 0 to 255
 * @param down - move from 255 - 0
 * @param push - push button change the state of the settings
 */
RotaryEncoder::RotaryEncoder(int up, int down, int push)
{
    _lastEncoded = Volume = _lastVolume = _lastPushState = SettingsState = 0;
    _maxSettingState = 3;

    pinMode(_up = up, INPUT);
    pinMode(_down = down, INPUT);
    pinMode(_push = push, INPUT);

    digitalWrite(_up, HIGH);
    digitalWrite(_down, HIGH);
}

/**
 * Handling rotary encoder to get values when rotating
 * It is handled by EncConstraint what max value of encoder will be
 * possible. For hours should be only 0 - 24, minutes 0 - 59, seconds 0 - 59
 * each RGB color should be 0 - 255
 */
void *RotaryEncoder::ServiceInterrupt()
{
    int signalA = digitalRead(_up);
    int signalB = digitalRead(_down);

    int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
    int sum = (_lastEncoded << 2) | encoded; // adding it to the previous encoded value

    if ((sum == 0b0111 || sum == 0b1110 || sum == 0b1000 || sum == 0b0001) && Volume < 255)
        Volume++;
    if ((sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) && Volume > 0)
        Volume--;

    _lastEncoded = encoded;
}

/**
 * Read rotary encoder and set states and volume
 */
void RotaryEncoder::Read()
{
    // when Volume is different then last one and
    // we are at setting mode then print Volume and set it
    // as last one
    if (Volume != _lastVolume && SettingsState != 0)
    {
        Serial.println(Volume);
        _lastVolume = Volume;
    }

    // read push pin
    int pushValue = digitalRead(_push);
    if (pushValue == LOW && _lastPushState != pushValue)
    {
        // increment settings state
        SettingsState++;

        // when setting state is over the max value,
        // thne reset (means close setting mode)
        if (_maxSettingState < SettingsState)
        {
            SettingsState = 0;
        }
        Volume = 0;
    }
    _lastPushState = pushValue;
}