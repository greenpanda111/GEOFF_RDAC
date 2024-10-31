#ifndef ENCODERS_H
#define ENCODERS_H

#include "mbed.h"

using namespace mbed;

#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

class Encoder
{
public:
    Encoder(PinName pinA, PinName pinB);
    void reset(void);
    int getRevA(void);
    int getRevB(void);
    void setup(void);

    //InterruptIn encoderA(_pinA);
    //InterruptIn encoderB(_pinB);

private:
    void countPulseA(void);
    void countPulseB(void);

    InterruptIn _pinA;
    InterruptIn _pinB;

    volatile long int _shaftRevA;
    volatile long int _shaftRevB;

    volatile long int _countA;
    volatile long int _countB;
};

extern Encoder encoder;

#endif // ENCODERS_H