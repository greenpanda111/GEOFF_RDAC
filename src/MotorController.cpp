
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"
#include "Map.h"
#include "Bumpers.h"

using namespace mbed;

#define KP 0.5
#define DEFAULTSPEED 0.4f
#define RADIUS 49.5

#define LEFT_FORWARD 0.5
#define RIGHT_FORWARD -0.5

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORWARD, leftEncoder);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD, rightEncoder);

MotorController motorControl(leftMotor, rightMotor);

Bumper bumper(GPIO_PIN_3, GPIO_PIN_2);

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotorController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _currentAngle = 0;
  bumper.setup();
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
  leftEncoder.reset();
  rightEncoder.reset();

  while ((abs(leftEncoder.getDistance()) < distance) & (abs(rightEncoder.getDistance()) < distance) & (_stuck == false))
  {
    int error = (leftEncoder.getDistance() - rightEncoder.getDistance()) * KP;

    _leftMotor.move(DEFAULTSPEED + error);
    _rightMotor.move(-(DEFAULTSPEED - error));
  }
  stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::reverseDist(int distance)
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

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::rotate(int angle)
{
  leftEncoder.reset();
  rightEncoder.reset();

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

      _leftMotor.move((DEFAULTSPEED - error));
      _rightMotor.move((DEFAULTSPEED + error));
    }
  }
  stop();

  updateCurrentAngle(angle);

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

float MotorController::arcLength(float angle)
{
  float arcLength = (2 * PI * RADIUS * (angle / 360));
  return arcLength;
}

void MotorController::updateCurrentAngle(int angleChange)
{
  _currentAngle += angleChange;
  if (_currentAngle >= 360)
  {
    _currentAngle = _currentAngle - 360;
  }
}

void MotorController::avoid(void)
{
  motorControl.setStuck(false);
  motorControl.stop();
  motorControl.reverseDist(40);
  Serial.println("avoiding");
}

void MotorController::stop()
{
  Serial.println("stopping");
  _leftMotor.stop();
  _rightMotor.stop();
}