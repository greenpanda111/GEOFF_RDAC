#ifndef Ultrasonics_h
#define Ultrasonics_h
#include "Arduino.h"

#include "mbed.h"
using namespace mbed;

#define LEFT_ULTRASONIC_SENSOR_PIN P1_13
#define RIGHT_ULTRASONIC_SENSOR_PIN P0_23

class Ultrasonic
{
public:
    Ultrasonic(PinName ultrasonicPin);
    void correction();
    int read();

private:
    DigitalInOut _ultrasonicPin;
    Timer _sonar;
    int _offset;
    int _distance;
};

extern Ultrasonic ultrasonic;

#endif