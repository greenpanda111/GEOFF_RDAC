#ifndef MotorClass_h
#define MotorClass_h
#include "Arduino.h"
#include "encoders.h"

#include "mbed.h"
using namespace mbed;

#define Right_Motor_PWM P0_27
#define Right_Motor_Direction P0_4
#define Left_Motor_PWM P1_2
#define Left_Motor_Direction P0_5
#define PWM_Period 0.01f
#define Default_Power 0.5f


class Motor
{
public:
  Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder);
  void move(float power);
  void setup();
  void stop();
  float getEncoderDist(void);
  float getEncoderCount(void);

private:
  Encoder &_encoder;
  PwmOut _PwmPin;
  DigitalOut _dirPin;
  Ticker _velocityTicker;
  bool _forwardDirection;
  float _lastEncoderDist;
  float _currentVelocity;
  void calculateCurrentVelocity(void);
};

extern Motor leftMotor;
extern Motor rightMotor;
#endif