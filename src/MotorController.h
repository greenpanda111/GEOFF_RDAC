
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
  void rotate(int angle);
  void stop();
  void setup();
  int getCurrentAngle();
  void setStuck(bool status);

private:
  Motor &_leftMotor;
  Motor &_rightMotor;

  float arcLength(float angle);
  void updateCurrentAngle(int angleChange);
  void avoid(void);

  bool _forwardDirection;
  int _currentAngle;
  bool _stuck;
};
extern MotorController motorControl;
#endif
