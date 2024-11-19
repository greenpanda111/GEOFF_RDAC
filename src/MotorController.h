
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
  void rotate(int angle);
  void stop();
  void setup();
  int getCurrentAngle();
  void setCurrentAngle(int angle);
  
private:
  Motor &_leftMotor;
  Motor &_rightMotor;
  float arcLength(float angle);
  void updateCurrentAngle(int angleChange);
  float _pwr;
  int _currentAngle;
};
extern MotorController motorControl;
#endif
