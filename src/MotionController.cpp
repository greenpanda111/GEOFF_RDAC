
#include "Arduino.h"
#include "MotorClass.h"
#include "MotionController.h"
#include "mbed.h"
#include "Map.h"
#include "Bumpers.h"

using namespace mbed;

#define KP 0.5
#define DEFAULTSPEED 0.4f
#define RADIUS 53

#define LEFT_FORWARD 0.5
#define RIGHT_FORWARD -0.5

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORWARD, leftEncoder);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD, rightEncoder);

MotionController motorControl(leftMotor, rightMotor);

Bumper bumper(GPIO_PIN_3, GPIO_PIN_2);

MotionController::MotionController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotionController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _currentAngle = 0;
  _align = false;
  bumper.setup();
}
int MotionController::getCurrentAngle()
{
  return _currentAngle;
}

void MotionController::setCurrentAngle(int angle)
{
  _currentAngle = angle;
}

void MotionController::setStuck(bool status)
{
  _stuck = status;
}

void MotionController::setAlign(bool status)
{
  _align = status;
}

void MotionController::forwardDist(int distance)
{
  leftEncoder.reset();
  rightEncoder.reset();

  while ((abs(leftEncoder.getDistance()) < distance) & (abs(rightEncoder.getDistance()) < distance) & (_stuck == false))
  {
    int error = (leftEncoder.getDistance() - rightEncoder.getDistance()) * KP;

    _leftMotor.move(DEFAULTSPEED + error);
    _rightMotor.move(-(DEFAULTSPEED - error));
  }
  stop();
  mapUpdateLocation(distance);
  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotionController::reverseDist(int distance)
{
  leftEncoder.reset();
  rightEncoder.reset();

  while ((abs(leftEncoder.getDistance()) < distance) & (abs(rightEncoder.getDistance()) < distance) & (_stuck == false))
  {
    int error = (leftEncoder.getDistance() - rightEncoder.getDistance()) * KP;

    _leftMotor.move(-(DEFAULTSPEED - error));
    _rightMotor.move((DEFAULTSPEED + error));
  }
  stop();
  mapUpdateLocation(distance);
  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotionController::rotate(int angle)
{
  leftEncoder.reset();
  rightEncoder.reset();
  updateCurrentAngle(angle);

  float distance = arcLength(abs(angle));

  if (angle > 0)
  {
    while ((abs(leftMotor.getEncoderDist()) < distance) & (abs(rightMotor.getEncoderDist()) < distance) & (_stuck == false))
    {
      int error = (leftEncoder.getDistance() + rightEncoder.getDistance()) * KP;

      _leftMotor.move((DEFAULTSPEED - error));
      _rightMotor.move((DEFAULTSPEED + error));
    }
  }
  else
  {
    while ((abs(leftMotor.getEncoderDist()) < distance) & (abs(rightMotor.getEncoderDist()) < distance) & (_stuck == false))
    {
      int error = (leftEncoder.getDistance() + rightEncoder.getDistance()) * KP;

      _leftMotor.move(-(DEFAULTSPEED + error));
      _rightMotor.move(-(DEFAULTSPEED - error));
    }
  }
  stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

float MotionController::arcLength(float angle)
{
  float arcLength = (2 * PI * RADIUS * (angle / 360));
  return arcLength;
}

void MotionController::updateCurrentAngle(int angleChange)
{
  if (_align == false)
  {
    _currentAngle += angleChange;
    if (_currentAngle >= 360)
    {
      _currentAngle = _currentAngle - 360;
    }
    else if (_currentAngle < 0)
    {
      _currentAngle = 360 - abs(_currentAngle);
    }
  }
  else
  {
    return;
  }
}

void MotionController::avoid(void)
{
  motorControl.setStuck(false);
  motorControl.stop();
  motorControl.reverseDist(50);
  drawObstacle();
}

void MotionController::stop()
{
  _leftMotor.stop();
  _rightMotor.stop();
}