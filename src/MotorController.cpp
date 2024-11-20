
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

#define LEFT_FORDWARD 1
#define RIGHT_FORWARD 0

Encoder _leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder _rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORDWARD);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD);

MotorController motorControl(leftMotor, rightMotor);

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
  _currentAngle = 0;
}
int MotorController::getCurrentAngle()
{
  return _currentAngle;
}

void MotorController::forwardDist(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() < distance)
  {
    _leftMotor.move(RIGHT_FORWARD);
    _rightMotor.move(LEFT_FORDWARD);
  }
  _leftMotor.stop();
  _rightMotor.stop();
}

void MotorController::reverseDist(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() > -distance)
  {
    _leftMotor.move(!RIGHT_FORWARD);
    _rightMotor.move(!LEFT_FORDWARD);
  }
  _leftMotor.stop();
  _rightMotor.stop();
}

void MotorController::forwardVelocity(int velocity)
{
  _leftMotor.setTargetVelocity(velocity);
  _rightMotor.setTargetVelocity(velocity);
}

void MotorController::reverseVelocity(int velocity)
{
  _leftMotor.setTargetVelocity(velocity);
  _rightMotor.setTargetVelocity(velocity);
}

void MotorController::rotate(int angle)
{
  _leftEncoder.reset();

  if (angle > 0)
  {
    while (-_leftEncoder.getDistance() < arcLength(angle))
    {
      _leftMotor.move(LEFT_FORDWARD);
      _rightMotor.move(!RIGHT_FORWARD);
    }
  }
  else
  {
    while (_leftEncoder.getDistance() > -arcLength(angle))
    {
      _leftMotor.move(!LEFT_FORDWARD);
      _rightMotor.move(RIGHT_FORWARD);
    }
  }
  _leftMotor.stop();
  _rightMotor.stop();
  updateCurrentAngle(angle);
}

float MotorController::arcLength(float angle)
{
  float arcLength = (2 * 3.14 * 44 * (angle / 360));
  return arcLength;
}
void MotorController::updateCurrentAngle(int angleChange)
{
  _currentAngle += angleChange;
  if (_currentAngle > 360)
  {
    _currentAngle = _currentAngle - 360;
  }
}
void MotorController::stop()
{
  _leftMotor.stop();
  _rightMotor.stop();
}