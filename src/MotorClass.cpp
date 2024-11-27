#include "Arduino.h"
#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define KP 0.1
#define KI 0
#define KD 0

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _encoder.setup();
  _PIDTicker.attach(callback(this, &Motor::PID), 0.1);
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), 0.1);
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

void Motor::calculateCurrentVelocity(void)
{
    float velocity = 0;
    velocity = _encoder.getDistance()/0.1;
    setCurrentVelocity(velocity);
    
}

void Motor::resetEcoder(void)
{
    _encoder.reset();
}

float Motor::getEncoderDist(void)
{
    return _encoder.getDistance();
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

float Motor::getCurrentVelocity(void)
{
   return _currentVelocity;
}
