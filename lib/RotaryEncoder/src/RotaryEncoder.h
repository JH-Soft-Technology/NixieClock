/**
 * RotaryEncoder.h - Library for handling rotary encoder to setup the clock
 * Created by Jiri Horalek, February 12, 2019
 * Released with MIT licence
 */
#ifndef RotaryEncoder_h
#define RotaryEncoder_h

class RotaryEncoder
{
  public:
    struct EncConstraint
    {
        boolean isColor;
        boolean isTime;
        boolean isHours;
        boolean isMinutes;
        boolean isSeconds;
    };
    int SettingsState;
    int Volume;
    RotaryEncoder(int up, int down, int push);
    void *ServiceInterrupt();
    void Read();

  private:
    int _up;
    int _down;
    int _push;
    int _lastEncoded;
    int _lastPushState;
    int _lastVolume;
    int _maxSettingState;
};

#endif