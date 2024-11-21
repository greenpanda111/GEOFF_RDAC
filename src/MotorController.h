
#ifndef MotorController_h
#define MotorController_h
#include "Arduino.h"
#include "MotorClass.h"
#include "mbed.h"

#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

using namespace mbed;

class MotorController
{
public:
  MotorController(Motor &leftMotor, Motor &rightMotor);
  void forwardDist(int distance);
  void reverseDist(int distance);
  void forwardVelocity(int velocity);
  void reverseVelocity(int velocity);
  void rotate(int angle);
  void stop();
  void setup();
  int getCurrentAngle();

private:
  Motor &_leftMotor;
  Motor &_rightMotor;
  bool _forwardDirection;
  float arcLength(float angle);
  void updateCurrentAngle(int angleChange);
  float _pwr;
  int _currentAngle;
};
extern MotorController motorControl;
#endif
