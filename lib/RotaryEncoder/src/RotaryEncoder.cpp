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
RotaryEncoder::RotaryEncoder(uint8_t up, uint8_t down, uint8_t push)
{
    _confirmed = true;

    _lastEncoded =
        Volume =
            _lastVolume =
                _lastPushState =
                    _lastBouncedTime = 0;

    _maxSettingState = 4;
    state = NONE;

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
void RotaryEncoder::ServiceInterrupt()
{
    int signalA = digitalRead(_up);
    int signalB = digitalRead(_down);

    int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
    int sum = (_lastEncoded << 2) | encoded; // adding it to the previous encoded value

    if ((sum == 0b0111 || sum == 0b1110 || sum == 0b1000 || sum == 0b0001) && Volume < SetEncoderLimit(state))
        Volume++;
    if ((sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) && Volume > 0)
        Volume--;

    _lastEncoded = encoded;
}

uint8_t RotaryEncoder::SetEncoderLimit(State s)
{
    switch (s)
    {
    case BRIGHTNESS:
        return BRIGHTNESS_LIMIT;
        break;
    case COLOR_RED:
    case COLOR_GREEN:
    case COLOR_BLUE:
        return COLOR_LIMIT;
        break;
    default:
        return 255;
        break;
    }
}

/**
 * Read rotary encoder and set states and volume
 */
void RotaryEncoder::Read()
{
    // when Volume is different then last one and
    // we are at setting mode then print Volume and set it
    // as last one
    if (Volume != _lastVolume && state != NONE)
    {
        _lastVolume = Volume;
    }

    // read push pin
    int pushValue = digitalRead(_push);
    if (pushValue == LOW && _lastPushState != pushValue)
    {
        Serial.print("Change state from: ");
        Serial.print(state);

        // increment settings state
        state = (State)((int)state + 1);

        Serial.print(" to: ");
        Serial.print(state);

        Serial.print(". And changing _confirm to: ");
        Serial.println(_confirmed);

        // when setting state is over the max value,
        // thne reset (means close setting mode)
        if (_maxSettingState < (int)state)
        {
            state = NONE;
        }
        Volume = 0;
    }
    if (_confirmed == true)
    {
        _lastPushState = pushValue;
    }

    // software debouncing of rotary encoder (1)
    delay(60);
}

/**
 * Setting volume from outside of the rotary encoder
 *
 * Sometimes we need to set up the Volume of rotary encoder from
 * outside of the object. We need set _lastVoluem as well as Volume, because
 * the comparisoning both values
 * @param volume - number of 0 - 255
 * @param state - state where confirm will be used
 */
bool RotaryEncoder::SetVolume(uint8_t volume, State state)
{
    _lastVolume = volume;
    Volume = volume;
    bool result = Confirm(2, state);

    delay(60); // software debouncing rotary encoder (2)

    return result;
}

/**
 * Confirm settings
 * @param seconds - how long does it take to confirm the settings
 * @param s - state where confirm will be used
 */
bool RotaryEncoder::Confirm(uint8_t seconds, State s)
{
    if (state == s && _confirmed == false)
    {
        long currentTime = millis();

        int pushValue = digitalRead(_push);
        if (pushValue != _lastPushState && _confirmed == false)
        {
            _lastBouncedTime = currentTime;
        }

        if (pushValue == LOW && (currentTime - _lastBouncedTime) > seconds * 1000)
        {
            _confirmed = true;
            return _confirmed;
        }
        _lastPushState = pushValue;
    }
    return false;
}
