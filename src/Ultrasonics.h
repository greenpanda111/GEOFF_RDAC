#ifndef Ultrasonics_h
#define Ultrasonics_h
#include "mbed.h"

using namespace mbed;

#define LEFT_ULTRASONIC_SENSOR_PIN P1_13
#define RIGHT_ULTRASONIC_SENSOR_PIN P0_23

class Ultrasonic
{
public:
    Ultrasonic(PinName ultrasonicPin);
    // Measure the software polling timer delay
    // Run at start up
    void correction();
    // Send a pulse and return the distance measured 
    int read();

private:
    DigitalInOut _ultrasonicPin;
    Timer _sonar;
    int _offset;
    int _distance;
};

extern Ultrasonic ultrasonic;

#endif // ULTRASONICS_H