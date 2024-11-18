#ifndef ENCODERS_H
#define ENCODERS_H

#include "mbed.h"

using namespace mbed;

#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

#define WHEEL_RADIUS 23.75
#define WHEEL_PI 3.14159
#define WHEEL_CIRCUMFERENCE 149.15
#define STEP_CONVERSION 340

class Encoder
{
public:
    Encoder(PinName pinA, PinName pinB);
    void setup(void);
    void reset(void);
    int getDistance(void);

private:
    void countPulse(void);

    InterruptIn _pinA;
    InterruptIn _pinB;

    volatile long int _count;
};

extern Encoder encoder;

#endif // ENCODERS_H