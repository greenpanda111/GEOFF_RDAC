#ifndef MotorClass_h
#define MotorClass_h
#include "Arduino.h"

#include "mbed.h"
using namespace mbed;

#define Right_Motor_PWM P1_2
#define Right_Motor_Direction P0_5
#define Left_Motor_PWM P0_27
#define Left_Motor_Direction P0_4
#define PWM_Period 0.01f
#define Default_Power 0.5f

class Motor {
public:
  Motor(PinName PwmPin, PinName dirPin);
  void move(int direction, float power);
  void setup();
  void stop();
private:
  PwmOut _PwmPin;
  DigitalOut _dirPin;
};

extern Motor leftMotor;
extern Motor rightMotor;
#endif