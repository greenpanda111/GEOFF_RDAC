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
    // Reset encoder count to 0
    void reset(void);
    // return the calcualted distance from the encoder count
    float getDistance(void);
    // return the count measured by the encoder
    long int getCount(void);

private:
    // ISR to be run when a pulse from pin A is received
    void countPulse(void);

    InterruptIn _pinA;
    InterruptIn _pinB;

    volatile long int _count;
};

extern Encoder encoder;

#endif // ENCODERS_H