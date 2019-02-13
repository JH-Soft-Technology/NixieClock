/**
 * SegmentToNumberConvertor.h - Library for converting number to segments visualization
 * Created by Jiri Horalek, February 13, 2019
 * Released with MIT licence
 */
#ifndef SegmentToNumberConvertor_h
#define SegmentToNumberConvertor_h

#include <Arduino.h>

struct Segments
{
    uint8_t array[3];
};

class SegmentToNumberConvertor
{
  public:
    SegmentToNumberConvertor();
    Segments Convert(uint8_t number);
};

#endif