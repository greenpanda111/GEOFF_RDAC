
#ifndef MotorController_h
#define MotorController_h
#include "Arduino.h"
#include "MotorClass.h"
#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

#define Left_Forward 1
#define Right_Forward 0

#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

class MotorController {
public:
  MotorController(Motor &leftMotor, Motor &rightMotor);
  void forward(int distance);
  void reverse(int distance);
  void rotateClockwise();
  void rotateCounterClockwise();
  void stop();
  void setup();
private:
  Motor &_leftMotor;
  Motor &_rightMotor;
  float _pwr;
};
extern MotorController motorControl;
#endif
