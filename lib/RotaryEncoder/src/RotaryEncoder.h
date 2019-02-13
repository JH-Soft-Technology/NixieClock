/**
 * RotaryEncoder.h - Library for handling rotary encoder to setup the clock
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
 */
#ifndef RotaryEncoder_h
#define RotaryEncoder_h

enum EncoderLimits
{
    COLOR_LIMIT = 255,
    BRIGHTNESS_LIMIT = 25,
    HOURS_LIMIT = 24,
    MINUTES_LIMIT = 59,
    SECONDS_LIMIT = 59
};

enum State
{
    NONE,
    BRIGHTNESS,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    TIME_HOURS,
    TIME_MINUTES,
    TIME_SECONDS,
};

class RotaryEncoder
{
  public:
    State state;
    uint8_t Volume;
    RotaryEncoder(uint8_t up, uint8_t down, uint8_t push);
    void ServiceInterrupt();
    void Read();
    bool SetVolume(uint8_t volume, State state);

  private:
    uint8_t _up;
    uint8_t _down;
    uint8_t _push;
    uint8_t _lastEncoded;
    uint8_t _lastPushState;
    uint8_t _lastVolume;
    uint8_t _maxSettingState;
    bool _confirmed;
    long _lastBouncedTime;
    long _debounceDelay = 60;
    bool Confirm(uint8_t seconds, State s);
    uint8_t SetEncoderLimit(State s);
};

#endif