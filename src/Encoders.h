#ifndef ENCODERS_H
#define ENCODERS_H

#include "mbed.h"

using namespace mbed;

#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

#define WHEEL_RADIUS 23.75
#define WHEEL_PI 3.14
#define WHEEL_CIRCUMFERENCE 149.15

class Encoder
{
public:
    Encoder(PinName pinA, PinName pinB);
    void reset(void);
    int getForwardDist(void);
    int getReverseDist(void);
    void setup(void);

private:
    void countPulseA(void);
    void countPulseB(void);
    int convertToDist(int);

    InterruptIn _pinA;
    InterruptIn _pinB;

    float _shaftRevA;
    float _shaftRevB;

    volatile long int _countA;
    volatile long int _countB;
};

extern Encoder encoder;

#endif // ENCODERS_H