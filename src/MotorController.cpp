
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"

using namespace mbed;

#define LEFT_FORWARD 0.5
#define RIGHT_FORWARD -0.5
#define LEFT_REVERSE -0.5
#define RIGHT_REVERSE 0.5

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORWARD, leftEncoder);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD, rightEncoder);

MotorController motorControl(leftMotor, rightMotor);

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotorController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _currentAngle = 0;
}
int MotorController::getCurrentAngle()
{
  return _currentAngle;
}

void MotorController::setStuck(bool status)
{
  _stuck = status;
}

void MotorController::forwardDist(int distance)
{
  leftMotor.resetEcoder();
  while ((leftMotor.getEncoderDist() < distance) & (_stuck == false))
  {
    _leftMotor.move(LEFT_FORWARD);
    _rightMotor.move(RIGHT_FORWARD);
  }
  _leftMotor.stop();
  _rightMotor.stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::reverseDist(int distance)
{
  leftMotor.resetEcoder();

  while ((leftMotor.getEncoderDist() > -distance) & (_stuck == false))
  {
    _leftMotor.move(LEFT_REVERSE);
    _rightMotor.move(RIGHT_REVERSE);
  }
  _leftMotor.stop();
  _rightMotor.stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::avoid(void)
{
  Serial.println("avoiding");
  wait_us(1000);
  motorControl.setStuck(false);
  motorControl.reverseDist(100);
  motorControl.rotate(90);
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
  leftMotor.resetEcoder();
  Serial.println("rotating");
  Serial.println(arcLength(angle));

  if (angle > 0)
  {
    while ((leftMotor.getEncoderDist() < arcLength(angle)) & (_stuck == false))
    {
      Serial.print(leftMotor.getEncoderDist());
      Serial.print(" of ");
      Serial.println(arcLength(angle));

      _leftMotor.move(LEFT_REVERSE);
      _rightMotor.move(RIGHT_FORWARD);
    }
    _leftMotor.stop();
    _rightMotor.stop();
  }
  if (angle < 0)
  {
    while ((leftMotor.getEncoderDist() > arcLength(angle)) & (_stuck == false))
    {
      _leftMotor.move(LEFT_FORWARD);
      _rightMotor.move(RIGHT_REVERSE);
    }
  }
  else{}
  _leftMotor.stop();
  _rightMotor.stop();
  updateCurrentAngle(angle);

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

float MotorController::arcLength(float angle)
{
  float arcLength = (2 * 3.14 * 49.5 * (angle / 360));
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