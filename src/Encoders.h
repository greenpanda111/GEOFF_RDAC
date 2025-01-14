#ifndef ENCODERS_H
#define ENCODERS_H

#include "mbed.h"

using namespace mbed;

#define STEP_CONVERSION 2.2

// Both pins are used on the encoder so that direction can be determined by the leading pin's square wave
class Encoder
{
public:
    Encoder(PinName pinA, PinName pinB);
    void reset(void);
    float getDistance(void);
    long int getCount(void);

private:
    void countPulse(void);

    InterruptIn _pinA;
    InterruptIn _pinB;
    
    volatile long int _count;
};

extern Encoder encoder;

#endif // ENCODERS_H