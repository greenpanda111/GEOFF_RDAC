
#ifndef Bumpers_h
#define Bumpers_h
#include "mbed.h"
#include "Arduino.h"
using namespace mbed;

#define GPIO_PIN_2 P0_28
#define GPIO_PIN_3 P0_29

class Bumper
{
public:
    Bumper(PinName left, PinName right);
    void setup(void);
    void avoid(void);
private:
    
    InterruptIn _left;
    InterruptIn _right;
};

extern Bumper bumper;

#endif