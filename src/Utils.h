////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <algorithm>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

/**
* Converts degrees to radians
*/
template<typename T> T DegToRad(T degrees)
{
    return static_cast<T>(M_PI / 180.0) * degrees;
}

/**
* Converts radians to degrees
*/
template<typename T> T RadToDeg(T radians)
{
    return static_cast<T>(180.0 / M_PI)*radians;
}

/**
* Case insensitive comparison between two strings
*/
inline bool IsStrEqual(const std::string& str1, const std::string& str2)
{
    return _stricmp(str1.c_str(), str2.c_str()) == 0;
}

/**
* Change the range a value is between
*/
template<typename T> T ConvertRange(T value,    
                                    T currentInner, 
                                    T currentOuter, 
                                    T newInner, 
                                    T newOuter)
{
    return ((value-currentInner) * ((newOuter - newInner) /
        (currentOuter-currentInner))) + newInner;
}

/**
* Clamp a value between min and max
*/
template<typename T> T Clamp(T value, T minValue, T maxValue)
{
    return std::min(std::max(value, minValue), maxValue);
}