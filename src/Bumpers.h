
#ifndef Bumpers_h
#define Bumpers_h
#include "mbed.h"
using namespace mbed;

#define GPIO_PIN_2 P0_28
#define GPIO_PIN_3 P0_29

// The bumper consists of two micro switches attached to the front of the robot
// When either are pressed the robot will reverse
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

#endif // BUMPERS_H