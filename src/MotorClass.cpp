#include "Arduino.h"
#include "MotorClass.h"

#include "mbed.h"
using namespace mbed;

#define KP 0.1
#define KI 0
#define KD 0

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _PIDTicker.attach(callback(this, &Motor::PID), 0.1);
}

void Motor::move(float power)
{

  if (power > 0)
  {
    _dirPin.write(_forwardDirection);
    _PwmPin.write(power);
  }
  else
  {
    _dirPin.write(!_forwardDirection);
    _PwmPin.write(abs(power));
  }
}

void Motor::stop(void)
{
  _PwmPin.write(0.0f);
  _PIDSet = false;
}

void Motor::PID()
{
  if (_PIDSet == false)
  {
    return;
  }

  float error = _targetDistance - _currentDistance;

  // proportional term
  float proportional = KP * error;

  // update last error
  _lastError = error;

  // compute PID output
  float output = proportional;

  // clamp output between 1 and -1
  if (output > 1.0f)
  {
    output = 1.0f;
  }
  if (output < -1.0f)
  {
    output = -1.0f;
  }
}

void Motor::setTargetDistance(float target)
{
  _targetDistance = target;
}

void Motor::setCurrentDistance(float current)
{
  _currentDistance = current;
}

void Motor::setTargetVelocity(float target)
{
  _targetVelocity = target;
}

void Motor::setCurrentVelocity(float current)
{
  _currentVelocity = current;
}
