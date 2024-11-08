
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
  _leftMotor.setup();
  _rightMotor.setup();
  _pwr = Default_Power;
}

void MotorController::forward()
{
  _leftMotor.move(Left_Forward, _pwr);
  _rightMotor.move(Right_Forward, _pwr);
}

void MotorController::reverse()
{
  _leftMotor.move(Right_Forward, _pwr);
  _rightMotor.move(Left_Forward, _pwr);
}

void MotorController::rotateClockwise()
{
  _leftMotor.move(Left_Forward, _pwr);
  _rightMotor.move(Left_Forward, _pwr);
}

void MotorController::rotateCounterClockwise()
{
  _leftMotor.move(Right_Forward, _pwr);
  _rightMotor.move(Right_Forward, _pwr);
}

void MotorController::stop()
{
  _leftMotor.move(Left_Forward, 0.0f);
  _rightMotor.move(Right_Forward, 0.0f);
}
