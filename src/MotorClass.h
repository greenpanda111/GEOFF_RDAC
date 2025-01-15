#ifndef MotorClass_h
#define MotorClass_h
#include "encoders.h"

#include "mbed.h"
using namespace mbed;

// motor pins
#define RIGHT_MOTOR_PWM P0_27
#define RIGHT_MOTOR_DIRECTION P0_4
#define LEFT_MOTOR_PWM P1_2
#define LEFT_MOTOR_DIRECTION P0_5

#define PWM_PERIOD 0.01f

class Motor
{
public:
  Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder);
  // Turns motors on with given PWM power
  void move(float power);
  // Initialise PWM and velocity ticker
  void setup();
  // Sets PWM value on motor to 0
  void stop();
  // Returns the distance the encoder has turned
  float getEncoderDist(void);
  // Returns the count of the encoder
  float getEncoderCount(void);

private:
  Encoder &_encoder;
  PwmOut _PwmPin;
  DigitalOut _dirPin;
  Ticker _velocityTicker;

  bool _forwardDirection;

  // Velocity calculation in ticker
  float _lastEncoderDist;
  float _currentVelocity;
  void calculateCurrentVelocity(void);
};

extern Motor leftMotor;
extern Motor rightMotor;

#endif // MOTORCLASS_H