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

enum MovementMode{
  STOP,
  CONSTANT_VELOCITY,
  EXACT_DISTANCE,
  ROTATING
};

class Motor
{
public:
  Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder);
  void move(float power);
  void setup();
  void stop();
  void PID();
  void setTargetDistance(float target);
  void setIsRotating(bool value);
  float getEncoderDist(void);
  float getEncoderCount(void);

private:
  Encoder &_encoder;
  PwmOut _PwmPin;
  DigitalOut _dirPin;
  Ticker _PIDTicker;
  Ticker _velocityTicker;

  bool _forwardDirection;

  float _PIDDistError;
  float _PIDVelError;
  float _PIDLastDistError;
  float _PIDLastVelError;
  float _lastEncoderDist;
  bool _PIDSet;
  bool _isRotating;

  float _PIDIntegral;
  float _PIDDerivative;
  float _PIDOutput;

  float _targetDistance;
  float _currentDistance;

  float _targetVelocity;
  float _currentVelocity;
  void calculateCurrentVelocity(void);

  MovementMode _movementMode;
};

extern Motor leftMotor;
extern Motor rightMotor;
#endif