/**
 * SegmentToNumberConvertor.cpp - Library for converting number to segments visualization
 *
 * Library is calculating only with number from 0 to 255.
 * It will be handling only tree segments
 *
 * Each segment has 20 led diods. We need to send position for
 * each segment in array struct.
 *
 * Created by Jiri Horalek, February 13, 2019
 * Released with MIT licence
*/

#include "SegmentToNumberConvertor.h"

SegmentToNumberConvertor::SegmentToNumberConvertor()
{
}

/**
 * Convert uint8_t number to the tree segment positions
 * represented in array of thre values
 * @param number - number which is going to be converted
 */
Segments SegmentToNumberConvertor::Convert(uint8_t number)
{
    Segments segments;

    String strNum = String(number);
    String first, second, third;
    uint8_t length = strNum.length();

    if (length == 1)
    {
        segments.array[0] = 0;
        segments.array[1] = 20;
        segments.array[2] = 40 + (strNum.toInt() * 2);
    }
    else if (length == 2)
    {
        first = strNum.charAt(0);
        second = strNum.charAt(1);

        segments.array[0] = 0;
        segments.array[1] = 20 + (first.toInt() * 2);
        segments.array[2] = 40 + (second.toInt() * 2);
    }
    else if (length == 3)
    {
        first = strNum.charAt(0);
        second = strNum.charAt(1);
        third = strNum.charAt(2);

        segments.array[0] = 0 + (first.toInt() * 2);
        segments.array[1] = 20 + (second.toInt() * 2);
        segments.array[2] = 40 + (third.toInt() * 2);
    }

    return segments;
}
