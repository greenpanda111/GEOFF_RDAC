#ifndef ENCODERS_H
#define ENCODERS_H

#include "mbed.h"

using namespace mbed;

#define WHEEL_RADIUS 23.75
#define WHEEL_PI 3.14159
#define WHEEL_CIRCUMFERENCE 149.15
#define STEP_CONVERSION 2.2
#define ONE_ROTATION_TICKS 330

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