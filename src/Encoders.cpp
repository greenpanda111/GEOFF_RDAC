#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

Encoder::Encoder(PinName pinA, PinName pinB)
    : _pinA(pinA), _pinB(pinB)
{
    _pinA.rise(callback(this, &Encoder::countPulse));
    reset();
}

float Encoder::getDistance(void)
{
    // converts count to distance in mm
    return (float)_count / (float)STEP_CONVERSION;
}

long int Encoder::getCount(void)
{
    return _count;
}

void Encoder::countPulse(void)
{
    // if pin A is leading
    if (_pinA != _pinB)
    {
        // the motor is spinning forward
        _count++;
    }
    // if pin B is leading
    else
    {
        // the motor is spinning backward
        _count--;
    }
}

void Encoder::reset(void)
{
    _count = 0.0;
}
