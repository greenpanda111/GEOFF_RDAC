
#ifndef MotorController_h
#define MotorController_h
#include "Arduino.h"
#include "MotorClass.h"
#include "mbed.h"
using namespace mbed;

#define Left_Forward 0
#define Right_Forward 1

class MotorController {
public:
  MotorController(Motor &leftMotor, Motor &rightMotor);
  void forward();
  void reverse();
  void rotateClockwise();
  void rotateCounterClockwise();
  void stop();
private:
  Motor &_leftMotor;
  Motor &_rightMotor;
  float _pwr;
};

extern MotorController motorControl;
#endif
