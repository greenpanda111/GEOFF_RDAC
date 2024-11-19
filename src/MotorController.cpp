
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

Encoder _leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder _rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotorController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _leftEncoder.setup();
  _rightEncoder.setup();
  _pwr = Default_Power;
}
void MotorController::forward(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() < distance)
  {
    Serial.print("encoder Dist: ");
    Serial.println(_leftEncoder.getDistance());

    _leftMotor.move(Right_Forward, _pwr);
    _rightMotor.move(Left_Forward, _pwr);
  }
  _leftMotor.stop();
  _rightMotor.stop();
}

void MotorController::reverse(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() > distance)
  {
    Serial.print("encoder Dist: ");
    Serial.println(_leftEncoder.getDistance());

    _leftMotor.move(Right_Forward, _pwr);
    _rightMotor.move(Left_Forward, _pwr);
  }
  _leftMotor.stop();
  _rightMotor.stop();
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

extern Encoder _leftEncoder;
extern Encoder _rightEncoder;